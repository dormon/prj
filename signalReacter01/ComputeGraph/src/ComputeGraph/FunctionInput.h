#pragma once

#include<ComputeGraph/Input.h>

class ComputeGraph::FunctionInput : public Input {
  public:
  std::shared_ptr<Function> fce;
  size_t outputId;
  FunctionInput(
      std::shared_ptr<Function> const& f,
      size_t i)
      : Input(InputKind::FUNCTION), fce(f), outputId(i) {}
  virtual std::shared_ptr<Resource> getResource() const override;
};

