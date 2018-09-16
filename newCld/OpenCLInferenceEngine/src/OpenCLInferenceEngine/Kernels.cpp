#include<OpenCLInferenceEngine/Kernels.h>

std::string const kernelsSources = R".(
#line 5

#define LINEAR__ACTIVATION 0
#define RELU____ACTIVATION 1
#define SOFTMAX_ACTIVATION 2

float softmax(float v){
  return exp(v);
}

float relu(float v){
  return v < 0.f ? 0.f : v;
}

kernel void conv2d(
    global       float* output    ,
    global const float* input     ,
    global const float* weights   ,
    global const float* biases    ,
           const uint   nofLayers ,
           const uint   nofFilters,
           const uint2  imageSize ,
           const uint2  kernelSize){

  //if(get_global_id(0) == 0 && get_global_id(1) == 0){
  //  printf("KERNEL conv2d nofLayers %u\n",nofLayers);
  //  printf("KERNEL conv2d nofFilters %u\n",nofFilters);
  //  printf("KERNEL conv2d imageSize %u %u\n",imageSize.x,imageSize.y);
  //  printf("KERNEL conv2d kernelSize %u %u\n",kernelSize.x,kernelSize.y);
  //  //for(uint l=0;l<nofLayers;++l)
  //  //  for(uint y=0;y<imageSize.y;++y)
  //  //    for(uint x=0;x<imageSize.x;++x)
  //  //       printf("KERNEL input: %f\n",input[l*imageSize.x*imageSize.y + y*imageSize.x + x]);
  //  //       
  //  //for(uint f=0;f<nofFilters;++f)
  //  //  for(uint l=0;l<nofLayers;++l)
  //  //    for(uint y=0;y<kernelSize.y;++y)
  //  //      for(uint x=0;x<kernelSize.x;++x){
  //  //        uint id = f*kernelSize.x*kernelSize.y*nofLayers + l*kernelSize.x*kernelSize.y + y*kernelSize.y + x;
  //  //        printf("KERNEL weights %u filter: %u layer: %u y: %u x: %u: %f\n",id,f,l,y,x,weights[id]);
  //  //      }

  //  //for(uint f=0;f<nofFilters;++f)
  //  //  for(uint y=0;y<imageSize.y;++y)
  //  //    for(uint x=0;x<imageSize.x;++x)
  //  //    output[f*imageSize.x*imageSize.y + y*imageSize.x + x] = 1000.f;
  //}

  const uint inputNofColumns    = imageSize.x                         ;
  const uint inputNofRows       = imageSize.y                         ;
  const uint inputColumnLength  = inputNofRows                        ;
  const uint inputRowLength     = inputNofColumns                     ;
  const uint inputLayerSize     = inputColumnLength * inputRowLength  ;

  const uint outputColumnId     = get_global_id(0)                    ;
  const uint outputRowId        = get_global_id(1)                    ;
  const uint outputRowLength    = inputRowLength                      ;
  const uint outputColumnLength = inputColumnLength                   ;
  const uint outputLayerSize    = outputColumnLength * outputRowLength;

  const uint kernelNofRows      = kernelSize.y                        ;
  const uint kernelNofColumns   = kernelSize.x                        ;
  const uint kernelRowLength    = kernelNofColumns                    ;
  const uint kernelColumnLength = kernelNofRows                       ;
  const uint kernelLayerSize    = kernelColumnLength * kernelRowLength;

  const uint filterSize         = nofLayers * kernelLayerSize         ;

  const uint halfKernelRowLength    = (kernelRowLength   -1)>>1;
  const uint halfKernelColumnLength = (kernelColumnLength-1)>>1;

  for(uint filterId = 0; filterId < nofFilters; ++filterId){

    float value = 0.f;

    for(uint layerId = 0; layerId < nofLayers; ++layerId){
      for(uint kernelRowId = 0; kernelRowId < kernelNofRows; ++kernelRowId){
        for(uint kernelColumnId = 0; kernelColumnId < kernelNofColumns; ++ kernelColumnId){

          const int inputColumnId = 
            (int)(outputColumnId     ) + 
            (int)(kernelColumnId     ) - 
            (int)(halfKernelRowLength);

          const int inputRowId    = 
            (int)(outputRowId           ) + 
            (int)(kernelRowId           ) - 
            (int)(halfKernelColumnLength);

          if(inputColumnId < 0 || inputColumnId >= inputNofColumns)continue;
          if(inputRowId    < 0 || inputRowId    >= inputNofRows   )continue;

          const uint inputId   = 
            (uint)(layerId      ) * inputLayerSize + 
            (uint)(inputRowId   ) * inputRowLength + 
            (uint)(inputColumnId);

          const uint weightsId = 
            filterId     * filterSize      + 
            layerId      * kernelLayerSize + 
            kernelRowId  * kernelRowLength + 
            kernelColumnId;

          value += input[inputId] * weights[weightsId];
        }
      }
    }

    const uint outputId = 
      filterId    * outputLayerSize + 
      outputRowId * outputRowLength + 
      outputColumnId;

    output[outputId] = value;
  }
}

kernel void bias(
    global       float* output    ,
    global const float* input     ,
    global const float* biases    ,
           const uint   nofLayers ,
           const uint2  imageSize ){

  const uint nofColumns   = imageSize.x             ;
  const uint nofRows      = imageSize.y             ;
  const uint columnLength = nofRows                 ;
  const uint rowLength    = nofColumns              ;
  const uint layerSize    = columnLength * rowLength;

  const uint columnId = get_global_id(0);
  const uint rowId    = get_global_id(1);

  if(columnId >= nofColumns || rowId >= nofRows)return;

  for(uint layerId = 0; layerId < nofLayers; ++layerId){
    const uint id = 
      layerId * layerSize + 
      rowId   * rowLength + 
      columnId;

    output[id] = input[id] + biases[layerId];
  }
}

kernel void batchNormalization(
    global       float* output    ,
    global const float* input     ,
    global const float* parameters,
           const uint   nofLayers ,
           const uint2  imageSize ,
           const float  epsilon   ){

  //if(get_global_id(0) == 0 && get_global_id(1) == 0){
  //  printf("batchNormalization nofLayers %u\n",nofLayers);
  //}

  const uint columnId = get_global_id(0);
  const uint rowId    = get_global_id(1);

  const uint nofRows      = imageSize.y             ;
  const uint nofColumns   = imageSize.x             ;
  const uint rowLength    = nofColumns              ;
  const uint columnLength = nofRows                 ;
  const uint layerSize    = columnLength * rowLength;

  if(rowId >= nofRows || columnId >= nofColumns)return;

  for(uint layerId = 0; layerId < nofLayers; ++layerId){
    const float gamma    = parameters[0*nofLayers + layerId];
    const float beta     = parameters[1*nofLayers + layerId];
    const float mean     = parameters[2*nofLayers + layerId];
    const float variance = parameters[3*nofLayers + layerId];

    //TODO A,B could be stored in parameters
    const float A = gamma/sqrt(variance + epsilon);
    const float B = -mean/sqrt(variance + epsilon)*gamma + beta;

    const uint id = 
      layerId * layerSize + 
      rowId   * rowLength +
      columnId;

    output[id] = input[id]*A + B;
  }
}

kernel void activation(
    global       float* output        ,
    global const float* input         ,
           const uint   nofLayers     ,
           const uint2  imageSize     ,
           const uint   activationType){


  //if(get_global_id(0) == 0 && get_global_id(1) == 0){
  //  printf("activation nofLayers %u\n",nofLayers);
  //}

  const uint columnId = get_global_id(0);
  const uint rowId    = get_global_id(1);

  const uint nofColumns = imageSize.x         ;
  const uint nofRows    = imageSize.y         ;
  const uint layerSize  = nofColumns * nofRows;
  const uint rowLength  = nofColumns          ;

  if(columnId >= nofColumns)return;
  if(rowId    >= nofRows   )return;

  for(uint layerId = 0; layerId < nofLayers; ++layerId){
    const uint id = 
      layerId * layerSize + 
      rowId   * rowLength +
      columnId;
    float value = input[id];

    if(activationType == RELU____ACTIVATION)
      value = relu(value);

    if(activationType == SOFTMAX_ACTIVATION)
      value = softmax(value);

    output[id] = value;
  }
}


kernel void maxPooling(
    global       float* output         ,
    global const float* input          ,
           const uint   nofLayers      ,
           const uint2  inputImageSize ,
           const uint2  outputImageSize,
           const uint2  stride         ){

  //if(get_global_id(0) == 0 && get_global_id(1) == 0){
  //  printf("maxPooling nofLayers %u\n",nofLayers);
  //}

  const uint outputRowId        = get_global_id(1) ;
  const uint outputColumnId     = get_global_id(0) ;
  const uint outputNofRows      = outputImageSize.y;
  const uint outputNofColumns   = outputImageSize.x;
  const uint outputRowLength    = outputNofColumns ;
  const uint outputColumnLength = outputNofRows    ;

  const uint inputNofRows      = inputImageSize.y;
  const uint inputNofColumns   = inputImageSize.x;
  const uint inputRowLength    = inputNofColumns ;
  const uint inputColumnLength = inputNofRows    ;

  if(outputRowId >= outputNofRows || outputColumnId >= outputNofColumns)return;
  
  const uint outputLayerSize = outputColumnLength * outputRowLength;
  const uint inputLayerSize  = inputColumnLength  * inputRowLength ;

  for(uint layer = 0; layer < nofLayers; ++layer){

    float maxVal = -10e10f;

    for(uint y = 0; y < stride.y; ++y){
      for(uint x = 0; x < stride.x; ++x){
        const uint inputRowId    = outputRowId   *stride.y + y;
        const uint inputColumnId = outputColumnId*stride.x + x;
        if(inputRowId >= inputNofRows || inputColumnId >= inputNofColumns)continue;

        const uint inputId = layer * inputLayerSize + inputRowId * inputRowLength + inputColumnId;
        maxVal = max(maxVal,input[inputId]);
      }
    }

    const uint outputId = layer * outputLayerSize +  outputRowId * outputRowLength + outputColumnId;
    output[outputId] = maxVal;
  }
}

kernel void globalMaxPooling(
    global       float* output        ,
    global const float* input         ,
           const uint   nofLayers     ,
           const uint2  inputImageSize){

  //if(get_global_id(0) == 0 && get_global_id(1) == 0){
  //  printf("globalMaxPooling nofLayers %u\n",nofLayers);
  //  printf("globalMaxPooling inputImageSize %u %u\n",inputImageSize.x,inputImageSize.y);
  //}

  const uint layer = get_global_id(0);
  if(layer >= nofLayers)return;

  //TODO use parallel reduction to accelerated this
  
  const uint nofColumns   = inputImageSize.x        ;
  const uint nofRows      = inputImageSize.y        ;
  const uint rowLength    = nofColumns              ;
  const uint columnLength = nofRows                 ;
  const uint layerSize    = columnLength * rowLength;

  float maxVal = -10e10f;
  for(uint rowId = 0; rowId < nofRows; ++rowId)
    for(uint columnId = 0; columnId < nofColumns; ++columnId){
      const uint inputId = layer * layerSize + rowId * rowLength + columnId;
      maxVal = max(maxVal,input[inputId]);
    }

  output[layer] = maxVal;
}

kernel void dense(
    global       float* output    ,
    global const float* input     ,
    global const float* weights   ,
    global const float* biases    ,
           const uint   inputSize ,
           const uint   outputSize,
           const uint   activation){

  //if(get_global_id(0) == 0 && get_global_id(1) == 0){
  //  printf("dense inputSize %u\n",inputSize);
  //}

  uint const outputId = get_global_id(0);

  if(outputId >= outputSize)return;


  float value = 0.f;
  for(uint inputId = 0; inputId < inputSize; ++inputId){
    const uint weightsId = outputId * inputSize + inputId;
    value += input[inputId] * weights[weightsId];
  }

  value += biases[outputId];

  if(activation == RELU____ACTIVATION){
    value = relu(value);
  }

  if(activation == SOFTMAX_ACTIVATION){
    value = softmax(value);
  }

  output[outputId] = value;
}

kernel void softmaxNormalization(
    global       float*output   ,
    global const float*input    ,
           const uint  inputSize){

  //if(get_global_id(0) == 0 && get_global_id(1) == 0){
  //  printf("softmaxNormalization inputSize %u\n",inputSize);
  //}

  //TODO
  //this should be done by parallel reduction
  float sumVal = 0;
  for(uint inputId = 0; inputId < inputSize; ++inputId)
    sumVal += input[inputId];

  //TODO
  //this should be done by multiple threads in separated kernel
  for(uint inputId = 0; inputId < inputSize; ++inputId)
    output[inputId] = input[inputId] / sumVal;
}



).";
