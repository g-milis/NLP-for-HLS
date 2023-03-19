for (int i = 0; i < SIZE; i++)
    {
        float density = variables[NVAR*i + VAR_DENSITY];

        float3 momentum;
        momentum.x = variables[NVAR*i + (VAR_MOMENTUM + 0)];
        momentum.y = variables[NVAR*i + (VAR_MOMENTUM + 1)];
        momentum.z = variables[NVAR*i + (VAR_MOMENTUM + 2)];

        float density_energy = variables[NVAR*i + VAR_DENSITY_ENERGY];
        float3 velocity;       compute_velocity(density, momentum, velocity);
        float speed_sqd      = compute_speed_sqd(velocity);
        float pressure       = compute_pressure(density, density_energy, speed_sqd);
        float speed_of_sound = compute_speed_of_sound(density, pressure);

        result[i] = float(0.5f) / (sqrt(areas[i]) * (sqrt(speed_sqd) + speed_of_sound));
    }
