#pragma once

#include<ComputeGraph/InputKind.h>

class ComputeGraph::Input {
  public:
  Input(InputKind const& k) : inputKind(k) {}
  virtual ~Input() {}
  InputKind getInputKind() const { return inputKind; }
  virtual std::shared_ptr<Resource> getResource() const = 0;

  protected:
  InputKind inputKind;
};

