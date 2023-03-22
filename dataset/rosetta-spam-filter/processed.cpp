








FeatureType dotProduct(FeatureType param[NUM_FEATURES],
                       DataType    feature[NUM_FEATURES])
{
      
  FeatureType result = 0;
L1:  DOT: for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++)
  {
L2:    DOT_INNER: for(int j = 0; j < PAR_FACTOR; j++) 
    {
      FeatureType term = param[i*PAR_FACTOR+j] * feature[i*PAR_FACTOR+j];
      result += term;
    }
  }
  return result;
}

FeatureType useLUT(FeatureType in) 
{ 
    IdxFixed index;
  if (in < 0)
  {
    in = -in;
    index = (IdxFixed)LUT_SIZE - (IdxFixed)(((TmpFixed)in) << (LUTIN_TWIDTH - LUTIN_IWIDTH));  
  } 
  else 
    index = ((TmpFixed)in) << (LUTIN_TWIDTH - LUTIN_IWIDTH);  
  return lut[index];
}

FeatureType Sigmoid(FeatureType exponent) 
{
    if (exponent > 4) 
    return 1.0;
  else if (exponent < -4) 
    return 0.0;
  else 
  {
    LutInFixed inLut = (LutInFixed)exponent;
    return useLUT(inLut);
  } 
}

void computeGradient(
    FeatureType grad[NUM_FEATURES],
    DataType    feature[NUM_FEATURES],
    FeatureType scale)
{
  L3:  GRAD: for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++)
  {
L4:    GRAD_INNER: for (int j = 0; j < PAR_FACTOR; j++)
      grad[i*PAR_FACTOR+j] = (scale * feature[i*PAR_FACTOR+j]);
  }
}

void updateParameter(
    FeatureType param[NUM_FEATURES],
    FeatureType grad[NUM_FEATURES],
    FeatureType scale)
{
  
L5:  UPDATE: for (int i = 0; i < NUM_FEATURES / PAR_FACTOR; i++)
  {
L6:    UPDATE_INNER: for (int j = 0; j < PAR_FACTOR; j++)
      param[i*PAR_FACTOR+j] += scale * grad[i*PAR_FACTOR+j];
  }
}

void read_data(VectorDataType  data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE], 
               DataType        training_instance[NUM_FEATURES],
               int training_id)
{
    const int unroll_factor = PAR_FACTOR;

L7:  READ_TRAINING_DATA: for (int i = 0; i < NUM_FEATURES / D_VECTOR_SIZE; i ++ )
  {
    VectorFeatureType tmp_data = data[training_id * NUM_FEATURES / D_VECTOR_SIZE + i];
L8:    READ_TRAINING_DATA_INNER: for (int j = 0; j < D_VECTOR_SIZE; j ++ )
      training_instance[i * D_VECTOR_SIZE + j].range(DTYPE_TWIDTH-1, 0) = tmp_data.range((j+1)*DTYPE_TWIDTH-1, j*DTYPE_TWIDTH);
  }
}

void compute(FeatureType theta_local[NUM_FEATURES], 
             LabelType label_local[NUM_TRAINING],
             DataType training_instance[NUM_FEATURES],
             int training_id)
{
  L9:  static FeatureType gradient[NUM_FEATURES];
  const int unroll_factor = PAR_FACTOR;

    FeatureType step = STEP_SIZE;

    FeatureType dot = dotProduct(theta_local, training_instance);
    FeatureType prob = Sigmoid(dot);
    LabelType training_label = label_local[training_id];
    computeGradient(gradient, training_instance, (prob-training_label));
    updateParameter(theta_local, gradient, -step);
}
 
extern "C"
{
  void SgdLR( VectorDataType    data[NUM_FEATURES * NUM_TRAINING / D_VECTOR_SIZE],
              VectorLabelType   label[NUM_TRAINING / L_VECTOR_SIZE],
              VectorFeatureType theta[NUM_FEATURES / F_VECTOR_SIZE])
  {
                            
        const int unroll_factor = PAR_FACTOR;

    L10:    static FeatureType theta_local[NUM_FEATURES];
    L11:    static LabelType   label_local[NUM_TRAINING];
    L12:    static DataType training_instance[NUM_FEATURES];

    L13:    PARAM_INIT: for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i ++ )
    {
      VectorFeatureType tmp_theta = theta[i];
L14:      PARAM_INIT_INNER: for (int j = 0; j < F_VECTOR_SIZE; j ++ )
        theta_local[i * F_VECTOR_SIZE + j].range(FTYPE_TWIDTH-1, 0) = tmp_theta.range((j+1)*FTYPE_TWIDTH-1, j*FTYPE_TWIDTH);
    }

    L15:    LABEL_CP: for (int i = 0; i < NUM_TRAINING / L_VECTOR_SIZE; i ++ )
    {
      VectorLabelType tmp_label = label[i];
L16:      LABEL_CP_INNER: for (int j = 0; j < L_VECTOR_SIZE; j ++ )
        label_local[i * L_VECTOR_SIZE + j].range(LTYPE_WIDTH-1, 0) = tmp_label.range((j+1)*LTYPE_WIDTH-1, j*LTYPE_WIDTH);
    }
  
        L17:    EPOCH: for (int epoch = 0; epoch < NUM_EPOCHS; epoch ++) 
    {
      L18:      TRAINING_INST: for( int training_id = 0; training_id < NUM_TRAINING; training_id ++ )
      {  
                        read_data(data, training_instance, training_id);
                compute(theta_local, label_local, training_instance, training_id);
      }
    }

    L19:    STREAM_OUT: for (int i = 0; i < NUM_FEATURES / F_VECTOR_SIZE; i ++ )
    {
      VectorFeatureType tmp_theta = 0;
L20:      STREAM_OUT_INNER: for (int j = 0; j < F_VECTOR_SIZE; j ++ )
        tmp_theta.range((j+1)*FTYPE_TWIDTH-1, j*FTYPE_TWIDTH) = theta_local[i * F_VECTOR_SIZE + j].range(FTYPE_TWIDTH-1, 0);
      theta[i] = tmp_theta;
    }
  }
}
