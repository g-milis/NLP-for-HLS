for (int i=0; i< 288; i++) {
        U8 coef = coeffs[i];
        if (i<num_coefs) coeff_stream.write( coef );        
    }
for (int n = 0; n < 1920*1080 ; n++) {
        U8 pix = src[n];
        if (x<width) pixel_stream.write( pix );
        if (x==(stride-1)) x=0; else x++;
     }
for (int n = 0; n < 1920*1080; n++) {
        U8 pix = (x<width) ? pixel_stream.read() : 0;
        dst[n] = pix;
        if (x==(stride-1)) x=0; else x++;
    }
U8 LineBuffer[FILTER_V_SIZE-1][MAX_IMAGE_WIDTH];
for (int n=0; n< 2087054; n++)
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
char coeffs[FILTER_V_SIZE][FILTER_H_SIZE];
for (int i=0; i<FILTER_V_SIZE; i++) {
L11:        for (int j=0; j<FILTER_H_SIZE; j++) {
            coeffs[i][j] = coeff_stream.read();
        }
    }
for (int y = 0; y < 1080; y++) 
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
