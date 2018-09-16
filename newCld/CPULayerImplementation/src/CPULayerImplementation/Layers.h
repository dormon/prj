#pragma once

#include <NetworkConfiguration/NetworkConfiguration.h>

namespace CPULayerImplementation {
void conv2d(Tensor<float>& output, Tensor<float> const& input,
            std::shared_ptr<cnnConf::Layer> const& conf);
void bias(Tensor<float>& output, Tensor<float> const& input,
          std::shared_ptr<cnnConf::Layer> const& conf);
void maxPooling(Tensor<float>& output, Tensor<float> const& input,
                std::shared_ptr<cnnConf::Layer> const& conf);
void globalMaxPooling(Tensor<float>& output, Tensor<float> const& input,
                      std::shared_ptr<cnnConf::Layer> const& conf);
void dense(Tensor<float>& output, Tensor<float> const& input,
           std::shared_ptr<cnnConf::Layer> const& conf);
void batchNormalization(Tensor<float>& output, Tensor<float> const& input,
                        std::shared_ptr<cnnConf::Layer> const& conf);
void activation2d(Tensor<float>& output, Tensor<float> const& input,
                  cnnConf::ActivationType const& activation);
void concatenate3d(
    Tensor<float>&                                            output,
    std::vector<std::reference_wrapper<Tensor<float>>> const& inputs,
    std::shared_ptr<cnnConf::Layer> const&                    conf);
}
