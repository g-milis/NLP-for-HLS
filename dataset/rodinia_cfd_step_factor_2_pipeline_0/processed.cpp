for (int i = 0; i < TILE_ROWS / PARA_FACTOR; i++) {

L2:        float density[PARA_FACTOR];
L3:        float3 momentum[PARA_FACTOR];
L4:        float density_energy[PARA_FACTOR];

        int iii = i * PARA_FACTOR;

        density[0] = variables      [NVAR*iii + VAR_DENSITY     +   0 + 0 ]; 
        momentum[0].x = variables   [NVAR*iii + (VAR_MOMENTUM   +   0 + 0)];
        momentum[0].y = variables[NVAR*iii + (VAR_MOMENTUM      +   1 + 0)];
        momentum[0].z = variables[NVAR*iii + (VAR_MOMENTUM      +   2 + 0)];
        density_energy[0] = variables[NVAR*iii + VAR_DENSITY_ENERGY   + 0 ];

L5:        for (int ii = 0; ii < PARA_FACTOR; ii++) {
            int iii = i * PARA_FACTOR + ii;
            float3 velocity;       compute_velocity(density[ii], momentum[ii], velocity);
            float speed_sqd = compute_speed_sqd(velocity);
            float pressure = compute_pressure(density[ii], density_energy[ii], speed_sqd);
            float speed_of_sound = compute_speed_of_sound(density[ii], pressure);
        
                result[iii] = float(0.5f) / (float)((float)sqrt((float)areas[iii]) * (float)((float)sqrt((float)speed_sqd) + (float)speed_of_sound));
        }
    }
float result_inner [TILE_ROWS];
float variables_inner [TILE_ROWS * NVAR];
float areas_inner [TILE_ROWS];
for (int k = 0; k < SIZE / TILE_ROWS; k++){
        
        memcpy(variables_inner, variables + k * TILE_ROWS * NVAR, sizeof(float) * TILE_ROWS * NVAR);
        memcpy(areas_inner, areas + k * TILE_ROWS, sizeof(float) * TILE_ROWS);

        cfd_step_factor(result_inner, variables_inner, areas_inner);

        memcpy(result + k * TILE_ROWS, result_inner, sizeof(float) * TILE_ROWS);
    }
