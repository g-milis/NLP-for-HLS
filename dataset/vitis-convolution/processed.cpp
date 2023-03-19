




void ReadFromMem(
        unsigned short       width,
        unsigned short       height,
        unsigned short       stride,
        const char          *coeffs,
        hls::stream<char>   &coeff_stream,
        const unsigned char *src,
        hls::stream<U8>     &pixel_stream )
{
    assert(stride <= MAX_IMAGE_WIDTH);
    assert(height <= MAX_IMAGE_HEIGHT);
    assert(stride%64 == 0);

    unsigned num_coefs = 15*15;     unsigned num_coefs_padded = (((num_coefs-1)/64)+1)*64; L1:    read_coefs: for (int i=0; i< 288; i++) {
        U8 coef = coeffs[i];
        if (i<num_coefs) coeff_stream.write( coef );        
    }

    stride = (stride/64)*64;     unsigned offset = 0;
    unsigned x = 0;
L2:    read_image: for (int n = 0; n < 1920*1080 ; n++) {
        U8 pix = src[n];
        if (x<width) pixel_stream.write( pix );
        if (x==(stride-1)) x=0; else x++;
     }
}


void WriteToMem(
        unsigned short       width,
        unsigned short       height,
        unsigned short       stride,
        hls::stream<U8>     &pixel_stream,
        unsigned char       *dst)
{
    assert(stride <= MAX_IMAGE_WIDTH);
    assert(height <= MAX_IMAGE_HEIGHT);
    assert(stride%64 == 0);

    stride = (stride/64)*64;     unsigned offset = 0;
    unsigned x = 0;
L3:    write_image: for (int n = 0; n < 1920*1080; n++) {
        U8 pix = (x<width) ? pixel_stream.read() : 0;
        dst[n] = pix;
        if (x==(stride-1)) x=0; else x++;
    }    
}


struct window {
    U8 pix[FILTER_V_SIZE][FILTER_H_SIZE];
};


void Window2D(
        unsigned short        width,
        unsigned short        height,
        hls::stream<U8>      &pixel_stream,
        hls::stream<window>  &window_stream)
{
    L4:    U8 LineBuffer[FILTER_V_SIZE-1][MAX_IMAGE_WIDTH];  

        window Window;

    unsigned col_ptr = 0;
    unsigned ramp_up = 1920*((FILTER_V_SIZE-1)/2)+(FILTER_H_SIZE-1)/2;     unsigned num_pixels = 1920*1080;    unsigned num_iterations = num_pixels + ramp_up;

    const unsigned max_iterations = MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT + MAX_IMAGE_WIDTH*((FILTER_V_SIZE-1)/2)+(FILTER_H_SIZE-1)/2;

    L5:    update_window: for (int n=0; n< 2087054; n++)
    {

                U8 new_pixel = (n<num_pixels) ? pixel_stream.read() : 0;

        L6:        for(int i = 0; i < FILTER_V_SIZE; i++) {
L7:            for(int j = 0; j < FILTER_H_SIZE-1; j++) {
                Window.pix[i][j] = Window.pix[i][j+1];
            }
            Window.pix[i][FILTER_H_SIZE-1] = (i<FILTER_V_SIZE-1) ? LineBuffer[i][col_ptr] : new_pixel;
        }

        L8:        for(int i = 0; i < FILTER_V_SIZE-2; i++) {
            LineBuffer[i][col_ptr] = LineBuffer[i+1][col_ptr];
        }
        LineBuffer[FILTER_V_SIZE-2][col_ptr] = new_pixel;

                if (col_ptr==(width-1)) {
            col_ptr = 0;
        } else {
            col_ptr++;
        }

                if (n>=ramp_up) {
            window_stream.write(Window);
        }

    }
}

void Filter2D(
        unsigned short       width,
        unsigned short       height,
        float                factor,
        short                bias,
        hls::stream<char>   &coeff_stream,
        hls::stream<window> &window_stream,
		hls::stream<U8>     &pixel_stream )
{
    assert(width  <= MAX_IMAGE_WIDTH);
    assert(height <= MAX_IMAGE_HEIGHT);

    L9:    char coeffs[FILTER_V_SIZE][FILTER_H_SIZE];

    L10:    load_coefs: for (int i=0; i<FILTER_V_SIZE; i++) {
L11:        for (int j=0; j<FILTER_H_SIZE; j++) {
            coeffs[i][j] = coeff_stream.read();
        }
    }

    L12:    apply_filter: for (int y = 0; y < 1080; y++) 
    {
L13:        for (int x = 0; x < 1920; x++) 
        {
                        window w = window_stream.read();

                        int sum = 0;
L14:            for(int row=0; row<FILTER_V_SIZE; row++) 
            {
L15:                for(int col=0; col<FILTER_H_SIZE; col++) 
                {
                    unsigned char pixel;
                    int xoffset = (x+col-(FILTER_H_SIZE/2));
                    int yoffset = (y+row-(FILTER_V_SIZE/2));
                                        if ( (xoffset<0) || (xoffset>=width) || (yoffset<0) || (yoffset>=height) ) {
                        pixel = 0;
                    } else {
                        pixel = w.pix[row][col];
                    }
                    sum += pixel*(char)coeffs[row][col];
                }
            }

                        unsigned char outpix = MIN(MAX((int(factor * sum)+bias), 0), 255);

                        pixel_stream.write(outpix);
        }
    }
}


extern "C" {

void Filter2DKernel(
        const char           coeffs[256],
        float                factor,
        short                bias,
        unsigned short       width,
        unsigned short       height,
        unsigned short       stride,
        const unsigned char  src[MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT],
        unsigned char        dst[MAX_IMAGE_WIDTH*MAX_IMAGE_HEIGHT])
  {
            

	    hls::stream<char,2>    coefs_stream;
    hls::stream<U8,2>      pixel_stream;
    hls::stream<window,3>  window_stream;     hls::stream<U8,64>     output_stream;

	    ReadFromMem(width, height, stride, coeffs, coefs_stream, src, pixel_stream);

        Window2D(width, height, pixel_stream, window_stream);

		Filter2D(width, height, factor, bias, coefs_stream, window_stream, output_stream);

		WriteToMem(width, height, stride, output_stream, dst);

  }

}
