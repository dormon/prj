#pragma once

#include <ComputeGraph/Statement.h>

class ComputeGraph::Function : public Statement {
  public:
  Function(std::vector<std::shared_ptr<ResourceType>> const& i,
           std::vector<std::shared_ptr<ResourceType>> const& o);
  virtual void execute() = 0;
  virtual void bindInput(size_t, std::shared_ptr<Resource> const&);
  virtual void bindOutput(size_t, std::shared_ptr<Resource> const&);
  std::shared_ptr<Resource> getInputResource(size_t i) const;
  std::shared_ptr<Resource> getOutputResource(size_t i) const;
  std::shared_ptr<ResourceType> getInputType(size_t i) const;
  std::shared_ptr<ResourceType> getOutputType(size_t i) const;
  size_t getInputTicks(size_t i) const;

  protected:
  std::vector<Input>                         inputs;
  std::vector<std::shared_ptr<Resource>>     outputs;
  std::vector<std::shared_ptr<ResourceType>> inputTypes;
  std::vector<std::shared_ptr<ResourceType>> outputTypes;
  virtual void                               compute() override final;
  void checkInputType(size_t i, std::shared_ptr<Resource> const& r) const;
  void checkOutputType(size_t i, std::shared_ptr<Resource> const& r) const;
  void unbindOutput(size_t i);
  void unbindInput(size_t i);
  bool areInputsDifferent();
  void updateSeenTicks();
};
