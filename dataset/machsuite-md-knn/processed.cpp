for (i = 0; i < nAtoms; i++){
             i_x = position_x[i];
             i_y = position_y[i];
             i_z = position_z[i];
             fx = 0;
             fy = 0;
             fz = 0;
L2:loop_j : for( j = 0; j < maxNeighbors; j++){
                          jidx = NL[i*maxNeighbors + j];
                          j_x = position_x[jidx];
             j_y = position_y[jidx];
             j_z = position_z[jidx];
                          delx = i_x - j_x;
             dely = i_y - j_y;
             delz = i_z - j_z;
             r2inv = 1.0/( delx*delx + dely*dely + delz*delz );
                          r6inv = r2inv * r2inv * r2inv;
             potential = r6inv*(lj1*r6inv - lj2);
                          force = r2inv*potential;
             fx += delx * force;
             fy += dely * force;
             fz += delz * force;
         }
                  force_x[i] = fx;
         force_y[i] = fy;
         force_z[i] = fz;
                  }
