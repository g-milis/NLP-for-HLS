for(i=0; i < possible_outputs; i++) {
        sum += exp(-activations[i]);
    }
for(i=0; i < possible_outputs; i++) {
        net_outputs[i] = exp(-activations[i])/sum;
    }
for( i = 0; i < 100 ; i++) {
        dactivations[i] = activations[i]*(1.0-activations[i]);
        activations[i] = 1.0/(1.0+exp(-activations[i]));
    }
for( i = 0; i < 100 ; i++){
        activations[i] = activations[i] + biases[i];
    }
for(j = 0; j < nodes_per_layer; j++){
        activations[j] = (TYPE)0.0;
L6:        for (i = 0; i < input_dimension; i++){
            activations[j] += weights[j*input_dimension + i] * input_sample[i];
        }
    }
for (i = 0; i < nodes_per_layer; i++){
        activations[i] = (TYPE)0.0;
L8:        for(j = 0; j < nodes_per_layer; j++){
            activations[i] += weights[i*nodes_per_layer + j] * input_activations[j];
        }
    }
for(j = 0; j < possible_outputs; j++){
        activations[j] = (TYPE)0.0;
L10:        for (i = 0; i < nodes_per_layer; i++){
            activations[j] += weights[j*nodes_per_layer + i] * input_activations[i];
        }
    }
for( i = 0; i < possible_outputs; i++){
        output_difference[i] = (((net_outputs[i]) - solutions[i]) * -1.0) * dactivations[i];
    }
for( i = 0; i < nodes_per_layer; i++) {
L13:        for( j = 0; j < possible_outputs; j++) {
            delta_weights3[i*possible_outputs + j] = last_activations[i] * output_difference[j];
        }
    }
for( i = 0; i < nodes_per_layer; i++) {
        oracle_activations[i] = (TYPE)0.0;
L15:        for( j = 0; j < possible_outputs; j++) {
            oracle_activations[i] += output_differences[j] * weights3[i*possible_outputs + j];
        }
        oracle_activations[i] = oracle_activations[i] * dactivations[i];
    }
for( i = 0; i < nodes_per_layer; i++) {
L17:        for( j = 0; j < nodes_per_layer; j++) {
            delta_weights2[i*nodes_per_layer + j] = last_activations[i] * output_difference[j];
        }
    }
for( i = 0; i < nodes_per_layer; i++) {
        oracle_activations[i] = (TYPE)0.0;
L19:        for( j = 0; j < nodes_per_layer; j++) {
            oracle_activations[i] += output_differences[j] * weights2[i*nodes_per_layer + j];
        }
        oracle_activations[i] = oracle_activations[i] * dactivations[i];
    }
for( i = 0; i < input_dimension; i++) {
L21:        for( j = 0; j < nodes_per_layer; j++) {
            delta_weights1[i*nodes_per_layer + j] = last_activations[i] * output_difference[j];
        }
    }
for(i=0; i < input_dimension; i++){
L23:        for(j = 0; j < nodes_per_layer; j++){
            weights1[i*nodes_per_layer + j] -= (d_weights1[i*nodes_per_layer + j] * learning_rate);
            norm += weights1[i*nodes_per_layer + j]*weights1[i*nodes_per_layer + j];
        }
    }
for(i=0; i < nodes_per_layer; i++){
        biases1[i] -= (d_biases1[i]*learning_rate);
        bias_norm += biases1[i]*biases1[i];
    }
for(i=0; i < input_dimension; i++){
L26:        for(j = 0; j < nodes_per_layer; j++){
            weights1[i*nodes_per_layer + j] = (weights1[i*nodes_per_layer + j]/norm);
        }
    }
for(i=0; i < nodes_per_layer; i++){
        biases1[i] = (biases1[i]/bias_norm);
    }
for(i=0; i < nodes_per_layer; i++){
L29:        for(j = 0; j < nodes_per_layer; j++){
            weights2[i*nodes_per_layer + j] -= (d_weights2[i*nodes_per_layer + j] * learning_rate);
            norm += weights2[i*nodes_per_layer + j]*weights2[i*nodes_per_layer + j];
        }
    }
for(i=0; i < nodes_per_layer; i++){
        biases2[i] -= (d_biases2[i]*learning_rate);
        bias_norm += biases2[i]*biases2[i];
    }
for(i=0; i < nodes_per_layer; i++){
L32:        for(j = 0; j < nodes_per_layer; j++){
            weights2[i*nodes_per_layer + j] = (weights2[i*nodes_per_layer + j]/norm);
        }
    }
for(i=0; i < nodes_per_layer; i++){
        biases2[i] = (biases2[i]/bias_norm);
    }
for(i=0; i < nodes_per_layer; i++){
L35:        for(j = 0; j < possible_outputs; j++){
            weights3[i*possible_outputs + j] -= (d_weights3[i*possible_outputs + j] * learning_rate);
            norm += weights3[i*possible_outputs + j]*weights3[i*possible_outputs + j];
        }
    }
for(i=0; i<possible_outputs;i++){
        biases3[i] -= d_biases3[i]*learning_rate;
        bias_norm += biases3[i]*biases3[i];
    }
for(i=0; i < nodes_per_layer; i++){
L38:        for(j = 0; j < possible_outputs; j++){
            weights3[i*possible_outputs + j] = (weights3[i*possible_outputs + j]/norm);
        }
    }
for(i=0; i < possible_outputs; i++){
        biases3[i] = (biases3[i]/bias_norm);
    }
