#pragma once

#include <ComputeGraph/Statement.h>

class ComputeGraph::Function : public Statement {
  public:
  Function(std::vector<std::shared_ptr<ResourceType>> const& i,
           std::vector<std::shared_ptr<ResourceType>> const& o);
  virtual void compute() = 0;
  virtual void bindInput(size_t, std::shared_ptr<Resource> const&);
  virtual void bindOutput(size_t, std::shared_ptr<Resource> const&);
  virtual void addInputFunction(std::shared_ptr<Function> const&);
  virtual void removeInputFunction(std::shared_ptr<Function> const&);
  virtual void operator()() override;
  std::shared_ptr<Resource> getInputResource(size_t i) const;
  std::shared_ptr<Resource> getOutputResource(size_t i) const;
  std::shared_ptr<ResourceType> getInputType(size_t i) const;
  std::shared_ptr<ResourceType> getOutputType(size_t i) const;
  size_t getInputTicks(size_t i) const;

  protected:
  std::set<std::shared_ptr<Function>>        inputFunctions;
  std::vector<Input>                         inputs;
  std::vector<std::shared_ptr<Resource>>     outputs;
  std::vector<std::shared_ptr<ResourceType>> inputTypes;
  std::vector<std::shared_ptr<ResourceType>> outputTypes;
  void unbindOutput(size_t i);
  void unbindInput(size_t i);
  virtual void react(Signal const& s) override;
  void computeInputs();
  bool areInputsDifferent();
  void updateSeenTicks();
};
