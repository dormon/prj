#include <ComputeGraph/Function.h>
#include <ComputeGraph/FunctionInput.h>
#include <ComputeGraph/Input.h>
#include <ComputeGraph/Resource.h>
#include <ComputeGraph/ResourceInput.h>

ComputeGraph::Function::Function(
    std::vector<std::shared_ptr<ResourceType>> const& i,
    std::vector<std::shared_ptr<ResourceType>> const& o)
    : Statement(StatementKind::FUNCTION),
      inputs(i.size()),
      outputs(o.size()),
      inputTypes(i),
      outputTypes(o) {}

void ComputeGraph::Function::bindInput(
    size_t i, std::shared_ptr<Function> const& f, size_t j) {
  if (!getInputType(i)->equal(f->getOutputType(j))) {
    std::stringstream ss;
    ss << "Cannot bind function to input: " << i << " it has diffrent type";
    throw std::invalid_argument(ss.str());
  }
  unbindInput(i);
  if (!f) return;
  addSource(&*f);
  addSource(&*f->getOutputResource(j));
  inputs.at(i) = std::make_shared<FunctionInput>(f, j);
}

void ComputeGraph::Function::bindInput(
    size_t i, std::shared_ptr<Resource> const& r) {
  if (!getInputType(i)->equal(r->getType())) {
    std::stringstream ss;
    ss << "Cannot bind resource to input: " << i << " it has different type";
    throw std::invalid_argument(ss.str());
  }
  unbindInput(i);
  if (!r) return;
  addSource(&*r);
  inputs.at(i) = std::make_shared<ResourceInput>(r);
}

void ComputeGraph::Function::bindOutput(
    size_t i, std::shared_ptr<Resource> const& r) {
  if (!(getOutputType(i)->equal(r->getType()))) {
    std::stringstream ss;
    ss << "Cannot bind resource to output: " << i << " it has different type";
    throw std::invalid_argument(ss.str());
  }
  unbindOutput(i);
  if (!r) return;
  addTarget(&*r);

  //this function could be signaling others
  //if we manually change resource r, others should be notified
  //we need to give them r as signaling source.
  for(auto const&thisTarget:getTargets())
    thisTarget.first->addSource(&*r);

  outputs.at(i) = r;
}

bool ComputeGraph::Function::operator()() {
  if (!recompute) return false;
  recompute = false;
  bool isAnyInputChanged = computeInputs();
  if (!isAnyInputChanged) return false;
  compute();
  // emit(Signal(Signal::UPDATE_TICKS));
  return true;
}

std::shared_ptr<ComputeGraph::Resource>
ComputeGraph::Function::getInputResource(size_t i) const {
  if (inputs.at(i) == nullptr) return nullptr;
  return inputs.at(i)->getResource();
}

std::shared_ptr<ComputeGraph::Resource>
ComputeGraph::Function::getOutputResource(size_t i) const {
  return outputs.at(i);
}

std::shared_ptr<ComputeGraph::ResourceType>
ComputeGraph::Function::getInputType(size_t i) const {
  return inputTypes.at(i);
}

std::shared_ptr<ComputeGraph::ResourceType>
ComputeGraph::Function::getOutputType(size_t i) const {
  return outputTypes.at(i);
}

void ComputeGraph::Function::unbindOutput(size_t i) {
  if (outputs.at(i) == nullptr) return;

  removeTarget(&*outputs.at(i));

  //this function could signalize others.
  //If we manually update this function output, the others should be notified.
  //Therefore the set of signaling targets for this function outputs is the same
  //as the set of signaling targets for this function.
  //When we unbind output resource, it should no longer signalize the same set of
  //signaling targets.
  for(auto const&thisTarget:getTargets())
    outputs.at(i)->removeTarget(&*thisTarget.first);

  outputs.at(i) = nullptr;
}

void ComputeGraph::Function::unbindInput(size_t i) {
  if (inputs.at(i) == nullptr) return;

  removeSource(&*getInputResource(i));
  auto const input = inputs.at(i);
  if(input->getInputKind() == InputKind::FUNCTION)
    removeSource(&*std::dynamic_pointer_cast<FunctionInput>(input)->fce);

  inputs.at(i) = nullptr;
}

void ComputeGraph::Function::react(Signal const& s) {
  switch (s.signalKind) {
    case SignalKind::PROPAGATE_RECOMPUTE:
      if (recompute) return;
      recompute = true;
      emit(s);
      break;
    default:
      break;
  }
}

bool ComputeGraph::Function::computeResourceInput(
    std::shared_ptr<ResourceInput> const& i) {
  bool changed = i->seenUpdateTicks < i->res->getTicks();
  return changed;
}

bool ComputeGraph::Function::computeFunctionInput(
    std::shared_ptr<FunctionInput> const& i) {
  return (*i->fce)();
}

bool ComputeGraph::Function::computeInputs() {
  bool isAnyInputChanged = false;
  for (auto const& i : inputs) {
    switch (i->getInputKind()) {
      case InputKind::RESOURCE:
        isAnyInputChanged |=
            computeResourceInput(std::dynamic_pointer_cast<ResourceInput>(i));
        break;
      case InputKind::FUNCTION:
        isAnyInputChanged |=
            computeFunctionInput(std::dynamic_pointer_cast<FunctionInput>(i));
        break;
    }
  }
  return isAnyInputChanged;
}

std::shared_ptr<ComputeGraph::Resource>
ComputeGraph::FunctionInput::getResource() const {
  return fce->getOutputResource(outputId);
}
