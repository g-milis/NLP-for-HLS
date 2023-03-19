float ff_variable[NVAR] = {1.399999976158142090, 1.680000066757202148, 0.000000000000000000, 0.000000000000000000, 3.507999897003173828};
for(int i = 0; i < TILE_ROWS / PARA_FACTOR; i++)
    {
L3:        for (int ii = 0; ii < PARA_FACTOR; ii++) {
            int iii = i * PARA_FACTOR + ii;
        int j, nb;
        float3 normal; float normal_len;
        float factor;

        float density_i = variables[NVAR*iii + VAR_DENSITY];
        float3 momentum_i;
        momentum_i.x = variables[NVAR*iii + (VAR_MOMENTUM+0)];
        momentum_i.y = variables[NVAR*iii + (VAR_MOMENTUM+1)];
        momentum_i.z = variables[NVAR*iii + (VAR_MOMENTUM+2)];
        float density_energy_i = variables[NVAR*iii + VAR_DENSITY_ENERGY];

        float3 velocity_i;                           compute_velocity(density_i, momentum_i, velocity_i);
        float speed_sqd_i                          = compute_speed_sqd(velocity_i);
        float speed_i                              = sqrt(speed_sqd_i);
        float pressure_i                           = compute_pressure(density_i, density_energy_i, speed_sqd_i);
        float speed_of_sound_i                     = compute_speed_of_sound(density_i, pressure_i);
        float3 fc_i_momentum_x, fc_i_momentum_y, fc_i_momentum_z;
        float3 fc_i_density_energy;

        fc_i_momentum_x.x = fc_momentum_x[iii*NDIM + 0];
        fc_i_momentum_x.y = fc_momentum_x[iii*NDIM + 1];
        fc_i_momentum_x.z = fc_momentum_x[iii*NDIM + 2];

        fc_i_momentum_y.x = fc_momentum_y[iii*NDIM + 0];
        fc_i_momentum_y.y = fc_momentum_y[iii*NDIM + 1];
        fc_i_momentum_y.z = fc_momentum_y[iii*NDIM + 2];

        fc_i_momentum_z.x = fc_momentum_z[iii*NDIM + 0];
        fc_i_momentum_z.y = fc_momentum_z[iii*NDIM + 1];
        fc_i_momentum_z.z = fc_momentum_z[iii*NDIM + 2];

        fc_i_density_energy.x = fc_density_energy[iii*NDIM + 0];
        fc_i_density_energy.y = fc_density_energy[iii*NDIM + 1];
        fc_i_density_energy.z = fc_density_energy[iii*NDIM + 2];

        float flux_i_density = float(0.0f);
        float3 flux_i_momentum;
        flux_i_momentum.x = float(0.0f);
        flux_i_momentum.y = float(0.0f);
        flux_i_momentum.z = float(0.0f);
        float flux_i_density_energy = float(0.0f);

        float3 velocity_nb;
        float density_nb, density_energy_nb;
        float3 momentum_nb;
        float3 fc_nb_momentum_x, fc_nb_momentum_y, fc_nb_momentum_z;
        float3 fc_nb_density_energy;
        float speed_sqd_nb, speed_of_sound_nb, pressure_nb;

L4:        for(j = 0; j < NNB; j++)
        {
            nb = elements_surrounding_elements[iii*NNB + j];
            normal.x = normals[(iii*NNB + j)*NDIM + 0];
            normal.y = normals[(iii*NNB + j)*NDIM + 1];
            normal.z = normals[(iii*NNB + j)*NDIM + 2];
            normal_len = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

            if(nb >= 0)                 {
                density_nb =        variables[nb*NVAR + VAR_DENSITY];
                momentum_nb.x =     variables[nb*NVAR + (VAR_MOMENTUM+0)];
                momentum_nb.y =     variables[nb*NVAR + (VAR_MOMENTUM+1)];
                momentum_nb.z =     variables[nb*NVAR + (VAR_MOMENTUM+2)];
                density_energy_nb = variables[nb*NVAR + VAR_DENSITY_ENERGY];
                                                    compute_velocity(density_nb, momentum_nb, velocity_nb);
                speed_sqd_nb                      = compute_speed_sqd(velocity_nb);
                pressure_nb                       = compute_pressure(density_nb, density_energy_nb, speed_sqd_nb);
                speed_of_sound_nb                 = compute_speed_of_sound(density_nb, pressure_nb);

                fc_nb_momentum_x.x = fc_momentum_x[nb*NDIM + 0];
                fc_nb_momentum_x.y = fc_momentum_x[nb*NDIM + 1];
                fc_nb_momentum_x.z = fc_momentum_x[nb*NDIM + 2];

                fc_nb_momentum_y.x = fc_momentum_y[nb*NDIM + 0];
                fc_nb_momentum_y.y = fc_momentum_y[nb*NDIM + 1];
                fc_nb_momentum_y.z = fc_momentum_y[nb*NDIM + 2];

                fc_nb_momentum_z.x = fc_momentum_z[nb*NDIM + 0];
                fc_nb_momentum_z.y = fc_momentum_z[nb*NDIM + 1];
                fc_nb_momentum_z.z = fc_momentum_z[nb*NDIM + 2];

                fc_nb_density_energy.x = fc_density_energy[nb*NDIM + 0];
                fc_nb_density_energy.y = fc_density_energy[nb*NDIM + 1];
                fc_nb_density_energy.z = fc_density_energy[nb*NDIM + 2];

                                factor = -normal_len*smoothing_coefficient*float(0.5f)*(speed_i + sqrt(speed_sqd_nb) + speed_of_sound_i + speed_of_sound_nb);
                flux_i_density += factor*(density_i-density_nb);
                flux_i_density_energy += factor*(density_energy_i-density_energy_nb);
                flux_i_momentum.x += factor*(momentum_i.x-momentum_nb.x);
                flux_i_momentum.y += factor*(momentum_i.y-momentum_nb.y);
                flux_i_momentum.z += factor*(momentum_i.z-momentum_nb.z);

                                factor = float(0.5f)*normal.x;
                flux_i_density += factor*(momentum_nb.x+momentum_i.x);
                flux_i_density_energy += factor*(fc_nb_density_energy.x+fc_i_density_energy.x);
                flux_i_momentum.x += factor*(fc_nb_momentum_x.x+fc_i_momentum_x.x);
                flux_i_momentum.y += factor*(fc_nb_momentum_y.x+fc_i_momentum_y.x);
                flux_i_momentum.z += factor*(fc_nb_momentum_z.x+fc_i_momentum_z.x);

                factor = float(0.5f)*normal.y;
                flux_i_density += factor*(momentum_nb.y+momentum_i.y);
                flux_i_density_energy += factor*(fc_nb_density_energy.y+fc_i_density_energy.y);
                flux_i_momentum.x += factor*(fc_nb_momentum_x.y+fc_i_momentum_x.y);
                flux_i_momentum.y += factor*(fc_nb_momentum_y.y+fc_i_momentum_y.y);
                flux_i_momentum.z += factor*(fc_nb_momentum_z.y+fc_i_momentum_z.y);

                factor = float(0.5f)*normal.z;
                flux_i_density += factor*(momentum_nb.z+momentum_i.z);
                flux_i_density_energy += factor*(fc_nb_density_energy.z+fc_i_density_energy.z);
                flux_i_momentum.x += factor*(fc_nb_momentum_x.z+fc_i_momentum_x.z);
                flux_i_momentum.y += factor*(fc_nb_momentum_y.z+fc_i_momentum_y.z);
                flux_i_momentum.z += factor*(fc_nb_momentum_z.z+fc_i_momentum_z.z);
            }
            else if(nb == -1)               {
                flux_i_momentum.x += normal.x*pressure_i;
                flux_i_momentum.y += normal.y*pressure_i;
                flux_i_momentum.z += normal.z*pressure_i;
            }
            else if(nb == -2)             {
                factor = float(0.5f)*normal.x;
                flux_i_density += factor*(ff_variable[VAR_MOMENTUM+0]+momentum_i.x);
                flux_i_density_energy += factor*(ff_fc_density_energy.x+fc_i_density_energy.x);
                flux_i_momentum.x += factor*(ff_fc_momentum_x.x + fc_i_momentum_x.x);
                flux_i_momentum.y += factor*(ff_fc_momentum_y.x + fc_i_momentum_y.x);
                flux_i_momentum.z += factor*(ff_fc_momentum_z.x + fc_i_momentum_z.x);

                factor = float(0.5f)*normal.y;
                flux_i_density += factor*(ff_variable[VAR_MOMENTUM+1]+momentum_i.y);
                flux_i_density_energy += factor*(ff_fc_density_energy.y+fc_i_density_energy.y);
                flux_i_momentum.x += factor*(ff_fc_momentum_x.y + fc_i_momentum_x.y);
                flux_i_momentum.y += factor*(ff_fc_momentum_y.y + fc_i_momentum_y.y);
                flux_i_momentum.z += factor*(ff_fc_momentum_z.y + fc_i_momentum_z.y);

                factor = float(0.5f)*normal.z;
                flux_i_density += factor*(ff_variable[VAR_MOMENTUM+2]+momentum_i.z);
                flux_i_density_energy += factor*(ff_fc_density_energy.z+fc_i_density_energy.z);
                flux_i_momentum.x += factor*(ff_fc_momentum_x.z + fc_i_momentum_x.z);
                flux_i_momentum.y += factor*(ff_fc_momentum_y.z + fc_i_momentum_y.z);
                flux_i_momentum.z += factor*(ff_fc_momentum_z.z + fc_i_momentum_z.z);

            }
        }

        result[iii*NVAR + VAR_DENSITY] = flux_i_density;
        result[iii*NVAR + (VAR_MOMENTUM+0)] = flux_i_momentum.x;
        result[iii*NVAR + (VAR_MOMENTUM+1)] = flux_i_momentum.y;
        result[iii*NVAR + (VAR_MOMENTUM+2)] = flux_i_momentum.z;
        result[iii*NVAR + VAR_DENSITY_ENERGY] = flux_i_density_energy;
    }
    }
float result_inner [TILE_ROWS * NVAR ];
float elements_surrounding_elements_inner [TILE_ROWS * NNB ];
float normals_inner [TILE_ROWS * NNB * NDIM ];
float variables_inner [SIZE * NVAR ];
float fc_momentum_x_inner [SIZE * NDIM ];
float fc_momentum_y_inner [SIZE * NDIM ];
float fc_momentum_z_inner [SIZE * NDIM ];
float fc_density_energy_inner [SIZE * NDIM ];
for (int k; k < SIZE / TILE_ROWS; k++) {

        memcpy(elements_surrounding_elements_inner, elements_surrounding_elements + k * TILE_ROWS  * NNB, sizeof(float) * TILE_ROWS  * NNB);
        memcpy(normals_inner, normals + k * TILE_ROWS  * NNB * NDIM, sizeof(float) * TILE_ROWS  * NNB * NDIM);
        
        cfd_flux(result_inner, elements_surrounding_elements_inner, normals_inner, variables_inner, fc_momentum_x_inner, fc_momentum_y_inner, fc_momentum_z_inner, fc_density_energy_inner);
        
        memcpy(result + k * TILE_ROWS  *              NVAR, result_inner, sizeof(float) * TILE_ROWS  *              NVAR);
        
    }
