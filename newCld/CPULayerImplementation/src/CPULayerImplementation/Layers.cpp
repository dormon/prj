#include <CPULayerImplementation/Layers.h>

using namespace CPULayerImplementation;

struct CONV2D {
  struct INPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
  struct OUTPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
};

struct ACTIVATION2D {
  struct INPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
  struct OUTPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
};

struct BATCHNORMALIZATION {
  struct INPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
  struct OUTPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
};

struct MAXPOOLING {
  struct INPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
  struct OUTPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
};

struct GLOBALMAXPOOLING {
  struct INPUT {
    constexpr static size_t DIMENSION = 3;
    struct AXES {
      constexpr static size_t WIDTH   = 0;
      constexpr static size_t HEIGHT  = 1;
      constexpr static size_t CHANNEL = 2;
    };
  };
  struct OUTPUT {
    constexpr static size_t DIMENSION = 1;
    struct AXES {
      constexpr static size_t CHANNEL = 0;
    };
  };
};

struct DENSE {
  struct INPUT {
    constexpr static size_t DIMENSION = 1;
    struct AXES {
      constexpr static size_t INPUT = 0;
    };
  };
  struct OUTPUT {
    constexpr static size_t DIMENSION = 1;
    struct AXES {
      constexpr static size_t OUTPUT = 0;
    };
  };
};

std::map<cnnConf::ActivationType,
         std::function<float(float)>> static const activationFunctions = {
    {cnnConf::LINEAR, [](float v) { return v; }},
    {cnnConf::RELU, [](float v) { return v < 0.f ? 0.f : v; }},
    {cnnConf::SOFTMAX, [](float v) { return std::exp(v); }},
};

void CPULayerImplementation::activation2d(
    Tensor<float> &output, Tensor<float> const &input,
    cnnConf::ActivationType const &activation) {
  size_t const inputRowLength = input.size[ACTIVATION2D::INPUT::AXES::WIDTH];
  size_t const inputColumnLength =
      input.size[ACTIVATION2D::INPUT::AXES::HEIGHT];
  size_t const inputNofChannels =
      input.size[ACTIVATION2D::INPUT::AXES::CHANNEL];
  size_t const inputNofColumns  = inputRowLength;
  size_t const inputNofRows     = inputColumnLength;
  size_t const inputChannelSize = inputColumnLength * inputRowLength;

  size_t const outputRowLength = output.size[ACTIVATION2D::OUTPUT::AXES::WIDTH];
  size_t const outputColumnLength =
      output.size[ACTIVATION2D::OUTPUT::AXES::HEIGHT];
  size_t const outputNofChannels =
      output.size[ACTIVATION2D::OUTPUT::AXES::CHANNEL];
  size_t const outputNofColumns  = outputRowLength;
  size_t const outputNofRows     = outputColumnLength;
  size_t const outputChannelSize = outputColumnLength * outputRowLength;

  assert(getDimension(input) == ACTIVATION2D::INPUT::DIMENSION);
  assert(getDimension(output) == ACTIVATION2D::OUTPUT::DIMENSION);
  assert(inputNofChannels == outputNofChannels);
  assert(inputChannelSize == outputChannelSize);
  assert(inputRowLength == outputRowLength);
  assert(inputColumnLength == outputColumnLength);
  static_cast<void>(inputNofChannels);
  static_cast<void>(inputChannelSize);
  static_cast<void>(inputNofColumns);
  static_cast<void>(inputNofRows);
  assert(activationFunctions.count(activation) > 0);

  auto const activationFunction = activationFunctions.at(activation);

  for (size_t channelId = 0; channelId < outputNofChannels; ++channelId)
    for (size_t rowId = 0; rowId < outputNofRows; ++rowId)
      for (size_t columnId = 0; columnId < outputNofColumns; ++columnId) {
        size_t const outputId =
            channelId * outputChannelSize + rowId * outputRowLength + columnId;
        size_t const inputId  = outputId;
        output.data[outputId] = activationFunction(input.data[inputId]);
      }
}

void CPULayerImplementation::conv2d(
    Tensor<float> &output, Tensor<float> const &input,
    std::shared_ptr<cnnConf::Layer> const &conf) {
  assert(getDimension(input) == CONV2D::INPUT::DIMENSION);
  assert(getDimension(output) == CONV2D::OUTPUT::DIMENSION);

  auto const conv2dLayer =
      std::dynamic_pointer_cast<cnnConf::Conv2D<float>>(conf);
  assert(conv2dLayer != nullptr);

  auto const &weights = conv2dLayer->getWeights();

  size_t const inputRowLength    = input.size[CONV2D::INPUT::AXES::WIDTH];
  size_t const inputColumnLength = input.size[CONV2D::INPUT::AXES::HEIGHT];
  size_t const inputNofChannels  = input.size[CONV2D::INPUT::AXES::CHANNEL];
  size_t const inputNofColumns   = inputRowLength;
  size_t const inputNofRows      = inputColumnLength;
  size_t const inputChannelSize  = inputColumnLength * inputRowLength;

  size_t const outputRowLength    = output.size[CONV2D::OUTPUT::AXES::WIDTH];
  size_t const outputColumnLength = output.size[CONV2D::OUTPUT::AXES::HEIGHT];
  size_t const outputNofChannels  = output.size[CONV2D::OUTPUT::AXES::CHANNEL];
  size_t const outputNofColumns   = outputRowLength;
  size_t const outputNofRows      = outputColumnLength;
  size_t const outputChannelSize  = outputColumnLength * outputRowLength;

  static_cast<void>(outputNofChannels);
  static_cast<void>(outputNofColumns);
  static_cast<void>(outputNofRows);

  size_t const kernelRowLength =
      weights.size[cnnConf::Conv2D<float>::WEIGHTS::AXES::WIDTH];
  size_t const kernelColumnLength =
      weights.size[cnnConf::Conv2D<float>::WEIGHTS::AXES::HEIGHT];
  size_t const kernelNofChannels =
      weights.size[cnnConf::Conv2D<float>::WEIGHTS::AXES::CHANNEL];
  size_t const kernelNofFilters =
      weights.size[cnnConf::Conv2D<float>::WEIGHTS::AXES::FILTER];
  size_t const kernelNofColumns  = kernelRowLength;
  size_t const kernelNofRows     = kernelColumnLength;
  size_t const kernelChannelSize = kernelColumnLength * kernelRowLength;
  size_t const kernelSize        = kernelNofChannels * kernelChannelSize;

  size_t const kernelHalfRowLength    = (kernelRowLength - 1) / 2;
  size_t const kernelHalfColumnLength = (kernelColumnLength - 1) / 2;

  assert(inputNofChannels == kernelNofChannels);
  assert(outputNofChannels == kernelNofFilters);

  assert(inputRowLength == outputRowLength);
  assert(inputColumnLength == outputColumnLength);

  static_cast<void>(inputNofChannels);

  // compute convolution
  for (size_t filterId = 0; filterId < kernelNofFilters; ++filterId) {
    for (size_t convolutionRowId = 0; convolutionRowId < inputNofRows;
         ++convolutionRowId) {
      for (size_t convolutionColumnId = 0;
           convolutionColumnId < inputNofColumns; ++convolutionColumnId) {
        float value = 0;
        for (size_t kernelChannelId = 0; kernelChannelId < kernelNofChannels;
             ++kernelChannelId) {
          for (size_t kernelRowId = 0; kernelRowId < kernelNofRows;
               ++kernelRowId) {
            for (size_t kernelColumnId = 0; kernelColumnId < kernelNofColumns;
                 ++kernelColumnId) {
              ssize_t const inputRowId =
                  convolutionRowId + kernelRowId - kernelHalfColumnLength;
              ssize_t const inputColumnId =
                  convolutionColumnId + kernelColumnId - kernelHalfRowLength;
              ssize_t const inputChannelId = kernelChannelId;
              if (inputRowId < 0 || inputColumnId < 0 ||
                  inputRowId >= static_cast<ssize_t>(inputNofRows) ||
                  inputColumnId >= static_cast<ssize_t>(inputNofColumns))
                continue;
              size_t const inputId = inputChannelId * inputChannelSize +
                                     inputRowId * inputRowLength +
                                     inputColumnId;
              size_t const weightsId =
                  filterId * kernelSize + kernelChannelId * kernelChannelSize +
                  kernelRowId * kernelRowLength + kernelColumnId;
              value += input.data[inputId] * weights.data[weightsId];
            }
          }
        }
        size_t const outputChannelId = filterId;
        size_t const outputColumnId  = convolutionColumnId;
        size_t const outputRowId     = convolutionRowId;
        size_t const outputId        = outputChannelId * outputChannelSize +
                                outputRowId * outputRowLength + outputColumnId;
        output.data[outputId] = value;
      }
    }
  }
}

void CPULayerImplementation::bias(Tensor<float> &                        output,
                                  Tensor<float> const &                  input,
                                  std::shared_ptr<cnnConf::Layer> const &conf) {
  assert(input.size == output.size);

  auto const inputDimension  = getDimension(input);
  auto const outputDimension = getDimension(output);

  assert(inputDimension == outputDimension);

  static_cast<void>(inputDimension);
  static_cast<void>(outputDimension);

  auto const biasLayer = std::dynamic_pointer_cast<cnnConf::Bias<float>>(conf);
  assert(biasLayer != nullptr);

  auto const biases        = biasLayer->getBiases();
  auto const biasDimension = getDimension(biases);
  assert(inputDimension == biasDimension);

  static_cast<void>(biasDimension);

  size_t const rowLength    = input.size[CONV2D::INPUT::AXES::WIDTH];
  size_t const columnLength = input.size[CONV2D::INPUT::AXES::HEIGHT];
  size_t const nofChannels  = input.size[CONV2D::INPUT::AXES::CHANNEL];
  size_t const nofColumns   = rowLength;
  size_t const nofRows      = columnLength;
  size_t const channelSize  = columnLength * rowLength;

  for (size_t channelId = 0; channelId < nofChannels; ++channelId)
    for (size_t rowId = 0; rowId < nofRows; ++rowId)
      for (size_t columnId = 0; columnId < nofColumns; ++columnId) {
        size_t const id =
            channelId * channelSize + rowId * rowLength + columnId;
        size_t const biasesId = channelId;
        output.data[id]       = input.data[id] + biases.data[biasesId];
      }
}

void CPULayerImplementation::concatenate3d(
    Tensor<float> &                                           output,
    std::vector<std::reference_wrapper<Tensor<float>>> const &inputs,
    std::shared_ptr<cnnConf::Layer> const &                   conf) {
  auto const concatenateLayer =
      std::dynamic_pointer_cast<cnnConf::Concatenate>(conf);
  assert(concatenateLayer != nullptr);

  auto const concatenateAxis = concatenateLayer->getAxis();
  auto const nofInputs       = inputs.size();

  auto const &outputSize = output.size;

  size_t accumulatedSize = 0;
  for (size_t inputLayerId = 0; inputLayerId < nofInputs; ++inputLayerId) {
    size_t      inputId[3];
    auto const &inputSize = inputs.at(inputLayerId).get().size;
    for (inputId[2] = 0; inputId[2] < inputSize[2]; ++inputId[2]) {
      for (inputId[1] = 0; inputId[1] < inputSize[1]; ++inputId[1]) {
        for (inputId[0] = 0; inputId[0] < inputSize[0]; ++inputId[0]) {
          auto outputId = inputId;
          outputId[concatenateAxis] += accumulatedSize;
          size_t const inputId1d = inputId[2] * inputSize[1] * inputSize[0] +
                                   inputId[1] * inputSize[0] + inputId[0];
          size_t const outputId1d =
              outputId[2] * outputSize[1] * outputSize[0] +
              outputId[1] * outputSize[0] + outputId[0];
          output.data.at(outputId1d) =
              inputs.at(inputLayerId).get().data.at(inputId1d);
        }
      }
    }
    accumulatedSize += inputSize[concatenateAxis];
  }
}

void CPULayerImplementation::maxPooling(
    Tensor<float> &output, Tensor<float> const &input,
    std::shared_ptr<cnnConf::Layer> const &conf) {
  auto const poolingLayer = std::dynamic_pointer_cast<cnnConf::Pooling>(conf);
  assert(poolingLayer != nullptr);

  auto const & stride  = poolingLayer->getStride();
  size_t const strideX = stride.at(0);
  size_t const strideY = stride.at(1);

  size_t const inputRowLength    = input.size[MAXPOOLING::INPUT::AXES::WIDTH];
  size_t const inputColumnLength = input.size[MAXPOOLING::INPUT::AXES::HEIGHT];
  size_t const inputNofChannels  = input.size[MAXPOOLING::INPUT::AXES::CHANNEL];
  size_t const inputNofColumns   = inputRowLength;
  size_t const inputNofRows      = inputColumnLength;
  size_t const inputChannelSize  = inputColumnLength * inputRowLength;

  size_t const outputRowLength = output.size[MAXPOOLING::OUTPUT::AXES::WIDTH];
  size_t const outputColumnLength =
      output.size[MAXPOOLING::OUTPUT::AXES::HEIGHT];
  size_t const outputNofChannels =
      output.size[MAXPOOLING::OUTPUT::AXES::CHANNEL];
  size_t const outputNofColumns  = outputRowLength;
  size_t const outputNofRows     = outputColumnLength;
  size_t const outputChannelSize = outputColumnLength * outputRowLength;

  assert((inputRowLength / strideX) == outputRowLength);
  assert((inputColumnLength / strideY) == outputColumnLength);
  assert(inputNofChannels == outputNofChannels);

  static_cast<void>(outputNofChannels);
  static_cast<void>(outputNofColumns);
  static_cast<void>(outputNofRows);

  for (size_t channelId = 0; channelId < inputNofChannels; ++channelId) {
    for (size_t rowId = 0; rowId < inputNofRows; rowId += strideY)
      for (size_t columnId = 0; columnId < inputNofColumns;
           columnId += strideX) {
        float maxVal = std::numeric_limits<float>::lowest();
        for (size_t poolingRowId = 0; poolingRowId < strideY; ++poolingRowId)
          for (size_t poolingColumnId = 0; poolingColumnId < strideX;
               ++poolingColumnId) {
            size_t const inputRowId    = rowId + poolingRowId;
            size_t const inputColumnId = columnId + poolingColumnId;
            if (inputRowId >= inputNofRows || inputColumnId >= inputNofColumns)
              continue;

            size_t const inputId = channelId * inputChannelSize +
                                   inputRowId * inputRowLength + inputColumnId;
            maxVal = std::max(maxVal, input.data[inputId]);
          }
        size_t const outputRowId    = rowId / strideY;
        size_t const outputColumnId = columnId / strideX;
        size_t const outputId       = channelId * outputChannelSize +
                                outputRowId * outputRowLength + outputColumnId;
        output.data[outputId] = maxVal;
      }
  }
}

void CPULayerImplementation::globalMaxPooling(
    Tensor<float> &output, Tensor<float> const &input,
    std::shared_ptr<cnnConf::Layer> const &conf) {
  assert(getDimension(input) == GLOBALMAXPOOLING::INPUT::DIMENSION);
  assert(getDimension(output) == GLOBALMAXPOOLING::OUTPUT::DIMENSION);

  size_t const inputRowLength =
      input.size[GLOBALMAXPOOLING::INPUT::AXES::WIDTH];
  size_t const inputColumnLength =
      input.size[GLOBALMAXPOOLING::INPUT::AXES::HEIGHT];
  size_t const inputNofChannels =
      input.size[GLOBALMAXPOOLING::INPUT::AXES::CHANNEL];
  size_t const inputNofColumns  = inputRowLength;
  size_t const inputNofRows     = inputColumnLength;
  size_t const inputChannelSize = inputColumnLength * inputRowLength;

  size_t const outputNofChannels =
      output.size[GLOBALMAXPOOLING::OUTPUT::AXES::CHANNEL];

  assert(inputNofChannels == outputNofChannels);
  static_cast<void>(outputNofChannels);

  auto const globalMaxPoolingLayer =
      std::dynamic_pointer_cast<cnnConf::GlobalMaxPooling>(conf);
  assert(globalMaxPoolingLayer != nullptr);

  for (size_t channelId = 0; channelId < inputNofChannels; ++channelId) {
    float value = std::numeric_limits<float>::lowest();
    for (size_t rowId = 0; rowId < inputNofRows; ++rowId)
      for (size_t columnId = 0; columnId < inputNofColumns; ++columnId) {
        size_t const inputId =
            channelId * inputChannelSize + rowId * inputRowLength + columnId;
        value = std::max(value, input.data[inputId]);
      }
    output.data[channelId] = value;
  }
}

void CPULayerImplementation::dense(
    Tensor<float> &output, Tensor<float> const &input,
    std::shared_ptr<cnnConf::Layer> const &conf) {
  assert(getDimension(input) == DENSE::INPUT::DIMENSION);
  assert(getDimension(output) == DENSE::OUTPUT::DIMENSION);

  size_t const inputLength  = input.size[DENSE::INPUT::AXES::INPUT];
  size_t const outputLength = output.size[DENSE::OUTPUT::AXES::OUTPUT];

  auto denseLayer = std::dynamic_pointer_cast<cnnConf::Dense<float>>(conf);
  assert(denseLayer != nullptr);

  auto const &weights    = denseLayer->getWeights();
  auto const &biases     = denseLayer->getBiases();
  auto const  activation = denseLayer->getActivation();

  auto const weightsInputLength =
      weights.size[cnnConf::Dense<float>::WEIGHTS::AXES::INPUT];
  auto const weightsOutputLength =
      weights.size[cnnConf::Dense<float>::WEIGHTS::AXES::OUTPUT];

  // std::cout << "weightsInputLength: " << weightsInputLength << std::endl;
  // std::cout << "inputLength: " << inputLength << std::endl;
  assert(inputLength == weightsInputLength);
  assert(outputLength == weightsOutputLength);

  static_cast<void>(weightsOutputLength);

  assert(activationFunctions.count(activation) > 0);
  auto const activationFunction = activationFunctions.at(activation);

  for (size_t outputId = 0; outputId < outputLength; ++outputId) {
    float value = 0;
    for (size_t inputId = 0; inputId < inputLength; ++inputId) {
      size_t const weightId = outputId * weightsInputLength + inputId;
      value += input.data[inputId] * weights.data[weightId];
    }
    size_t const biasId = outputId;
    value += biases.data[biasId];
    value                 = activationFunction(value);
    output.data[outputId] = value;
  }

  // softmax normalization
  if (activation == cnnConf::SOFTMAX) {
    float sum = 0.f;
    for (size_t outputId = 0; outputId < outputLength; ++outputId)
      sum += output.data[outputId];
    for (size_t outputId = 0; outputId < outputLength; ++outputId)
      output.data[outputId] /= sum;
  }
}

void CPULayerImplementation::batchNormalization(
    Tensor<float> &output, Tensor<float> const &input,
    std::shared_ptr<cnnConf::Layer> const &conf) {
  assert(getDimension(input) == BATCHNORMALIZATION::INPUT::DIMENSION);
  assert(getDimension(output) == BATCHNORMALIZATION::OUTPUT::DIMENSION);

  size_t const inputRowLength =
      input.size[BATCHNORMALIZATION::INPUT::AXES::WIDTH];
  size_t const inputColumnLength =
      input.size[BATCHNORMALIZATION::INPUT::AXES::HEIGHT];
  size_t const inputNofChannels =
      input.size[BATCHNORMALIZATION::INPUT::AXES::CHANNEL];
  size_t const inputNofColumns  = inputRowLength;
  size_t const inputNofRows     = inputColumnLength;
  size_t const inputChannelSize = inputColumnLength * inputRowLength;

  size_t const outputRowLength =
      output.size[BATCHNORMALIZATION::OUTPUT::AXES::WIDTH];
  size_t const outputColumnLength =
      output.size[BATCHNORMALIZATION::OUTPUT::AXES::HEIGHT];
  size_t const outputNofChannels =
      output.size[BATCHNORMALIZATION::OUTPUT::AXES::CHANNEL];
  size_t const outputChannelSize = outputColumnLength * outputRowLength;

  assert(inputRowLength == outputRowLength);
  assert(inputColumnLength == outputColumnLength);
  assert(inputNofChannels == outputNofChannels);

  static_cast<void>(outputNofChannels);

  auto const batchNorm =
      std::dynamic_pointer_cast<cnnConf::BatchNormalization<float>>(conf);
  assert(batchNorm != nullptr);

  auto const  epsilon    = batchNorm->getEpsilon();
  auto const &parameters = batchNorm->getParameters();

  auto parametersNofChannels =
      parameters
          .size[cnnConf::BatchNormalization<float>::PARAMETERS::AXES::CHANNEL];

  assert(parametersNofChannels == inputNofChannels);

  for (size_t channelId = 0; channelId < inputNofChannels; ++channelId) {
    size_t const gammaId =
        cnnConf::BatchNormalization<float>::PARAMETERS::GAMMA *
            parametersNofChannels +
        channelId;
    size_t const betaId = cnnConf::BatchNormalization<float>::PARAMETERS::BETA *
                              parametersNofChannels +
                          channelId;
    size_t const meanId = cnnConf::BatchNormalization<float>::PARAMETERS::MEAN *
                              parametersNofChannels +
                          channelId;
    size_t const varianceId =
        cnnConf::BatchNormalization<float>::PARAMETERS::VARIANCE *
            parametersNofChannels +
        channelId;
    float const gamma    = parameters.data[gammaId];
    float const beta     = parameters.data[betaId];
    float const mean     = parameters.data[meanId];
    float const variance = parameters.data[varianceId];
    for (size_t rowId = 0; rowId < inputNofRows; ++rowId) {
      for (size_t columnId = 0; columnId < inputNofColumns; ++columnId) {
        auto const batchNormalizationFuncion = [&](float x) {
          float const A = gamma / std::sqrt(variance + epsilon);
          float const B = -mean / std::sqrt(variance + epsilon) * gamma + beta;
          return x * A + B;
        };
        size_t const outputId =
            channelId * outputChannelSize + rowId * outputRowLength + columnId;
        size_t const inputId =
            channelId * inputChannelSize + rowId * inputRowLength + columnId;
        output.data[outputId] = batchNormalizationFuncion(input.data[inputId]);
      }
    }
  }
}
