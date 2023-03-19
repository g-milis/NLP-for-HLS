for (int i = 0; i < TILE_ROWS; i++)
    {
     float density = variables[NVAR*i + VAR_DENSITY];

     float3 momentum;
     momentum.x = variables[NVAR*i + (VAR_MOMENTUM + 0)];
     momentum.y = variables[NVAR*i + (VAR_MOMENTUM + 1)];
     momentum.z = variables[NVAR*i + (VAR_MOMENTUM + 2)];

     float density_energy = variables[NVAR*i + VAR_DENSITY_ENERGY];
     float3 velocity;       compute_velocity(density, momentum, velocity);
     float speed_sqd = compute_speed_sqd(velocity);
     float pressure = compute_pressure(density, density_energy, speed_sqd);
     float speed_of_sound = compute_speed_of_sound(density, pressure);

     result[i] = float(0.5f) / (sqrt(areas[i]) * (sqrt(speed_sqd) + speed_of_sound));
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
