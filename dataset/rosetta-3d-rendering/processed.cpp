for ( bit16 k = 0; k < 100 ; k++ )
  {
    bit8 x = max_min[0] + k%max_min[4];
    bit8 y = max_min[2] + k/max_min[4];

    if( pixel_in_triangle( x, y, triangle_2d_same ) )
    {
      fragment2[i].x = x;
      fragment2[i].y = y;
      fragment2[i].z = triangle_2d_same.z;
      fragment2[i].color = color;
      i++;
    }
  }
static bit8 z_buffer[MAX_X][MAX_Y];
for ( bit16 i = 0; i < MAX_X; i++)
    {
L4:      ZCULLING_INIT_COL: for ( bit16 j = 0; j < MAX_Y; j++)
      {
        z_buffer[i][j] = 255;
      }
    }
for ( bit16 n = 0; n < 100 ; n++ ) 
  {
    if( fragments[n].z < z_buffer[fragments[n].y][fragments[n].x] )
    {
      pixels[pixel_cntr].x     = fragments[n].x;
      pixels[pixel_cntr].y     = fragments[n].y;
      pixels[pixel_cntr].color = fragments[n].color;
      pixel_cntr++;
      z_buffer[fragments[n].y][fragments[n].x] = fragments[n].z;
    }
  }
for ( bit16 i = 0; i < MAX_X; i++)
    {
L7:      COLORING_FB_INIT_COL: for ( bit16 j = 0; j < MAX_Y; j++)
        frame_buffer[i][j] = 0;
    }
for ( bit16 j = 0; j < MAX_Y; j++)
        frame_buffer[i][j] = 0;
    }
  }

  L8:  COLORING_FB: for ( bit16 i = 0; i < 10000 ; i++)
  {
    frame_buffer[ pixels[i].x ][ pixels[i].y ] = pixels[i].color;
  }
for ( bit16 i = 0; i < MAX_X; i++)
  {
L10:    OUTPUT_FB_COL: for ( bit16 j = 0; j < MAX_Y; j = j + 4)
    {
      out_FB( 7,  0) = frame_buffer[i][j + 0];
      out_FB(15,  8) = frame_buffer[i][j + 1];
      out_FB(23, 16) = frame_buffer[i][j + 2];
      out_FB(31, 24) = frame_buffer[i][j + 3];
      output[i * MAX_Y / 4 + j / 4] = out_FB;
    }
  }
CandidatePixel fragment[500];
Pixel pixels[500];
bit8 frame_buffer[MAX_X][MAX_Y];
bit8 max_min[5];
bit16 max_index[1];
for (bit16 i = 0; i < NUM_3D_TRI; i++)
    {
      bit32 input_lo  = input[3*i];
      bit32 input_mi  = input[3*i+1];
      bit32 input_hi  = input[3*i+2];
  
      triangle_3ds.x0 = input_lo( 7,  0);
      triangle_3ds.y0 = input_lo(15,  8);
      triangle_3ds.z0 = input_lo(23, 16);
      triangle_3ds.x1 = input_lo(31, 24);
      triangle_3ds.y1 = input_mi( 7,  0);
      triangle_3ds.z1 = input_mi(15,  8);
      triangle_3ds.x2 = input_mi(23, 16);
      triangle_3ds.y2 = input_mi(31, 24);
      triangle_3ds.z2 = input_hi( 7,  0);

                          
            projection( triangle_3ds, &triangle_2ds, angle );
      flag = rasterization1( triangle_2ds, max_min, &triangle_2ds_same, max_index);
      size_fragment = rasterization2( flag, max_min, max_index, triangle_2ds_same, fragment );
      size_pixels = zculling( i, fragment, size_fragment, pixels);
      coloringFB ( i, size_pixels, pixels, frame_buffer);
    }
