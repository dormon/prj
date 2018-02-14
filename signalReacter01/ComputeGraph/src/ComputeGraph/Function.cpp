#include <ComputeGraph/Function.h>
#include <ComputeGraph/Input.h>
#include <ComputeGraph/Resource.h>

using namespace ComputeGraph;

Function::Function(std::vector<std::shared_ptr<ResourceType>> const& i,
                   std::vector<std::shared_ptr<ResourceType>> const& o)
    : Statement(StatementKind::FUNCTION),
      inputs(i.size()),
      outputs(o.size()),
      inputTypes(i),
      outputTypes(o) {}

void Function::bindInput(size_t i, std::shared_ptr<Resource> const& r) {
  if (!r) {
    unbindInput(i);
    return;
  }
  if (!getInputType(i)->equal(r->getType())) {
    std::stringstream ss;
    ss << "Cannot bind resource to input: " << i << " it has different type";
    throw std::invalid_argument(ss.str());
  }
  unbindInput(i);
  addSource(&*r);
  inputs.at(i).resource  = r;
  inputs.at(i).seenTicks = r->getTicks() - 1;
  recompute              = true;
}

void Function::bindOutput(size_t i, std::shared_ptr<Resource> const& r) {
  if (!r) {
    unbindOutput(i);
    return;
  }
  if (!(getOutputType(i)->equal(r->getType()))) {
    std::stringstream ss;
    ss << "Cannot bind resource to output: " << i << " it has different type";
    throw std::invalid_argument(ss.str());
  }
  unbindOutput(i);
  addTarget(&*r);
  outputs.at(i) = r;
  recompute     = true;
}

void Function::addInputFunction(std::shared_ptr<Function> const& f) {
  inputFunctions.insert(f);
}

void Function::removeInputFunction(std::shared_ptr<Function> const& f) {
  auto const it = inputFunctions.find(f);
  if (it == inputFunctions.cend()) return;
  inputFunctions.erase(it);
}

std::shared_ptr<Resource> Function::getInputResource(size_t i) const {
  return inputs.at(i).resource;
}

std::shared_ptr<Resource> Function::getOutputResource(size_t i) const {
  return outputs.at(i);
}

std::shared_ptr<ResourceType> Function::getInputType(size_t i) const {
  return inputTypes.at(i);
}

std::shared_ptr<ResourceType> Function::getOutputType(size_t i) const {
  return outputTypes.at(i);
}

void Function::unbindOutput(size_t i) {
  if (outputs.at(i) == nullptr) return;

  removeTarget(&*outputs.at(i));

  outputs.at(i) = nullptr;
}

void Function::unbindInput(size_t i) {
  if (inputs.at(i).resource == nullptr) return;

  removeSource(&*getInputResource(i));

  inputs.at(i).resource = nullptr;
}

void Function::react(Signal const& s) {
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

bool Function::areInputsDifferent() {
  bool isAnyInputChanged = false;
  for (auto const& i : inputs)
    isAnyInputChanged |= i.seenTicks < i.resource->getTicks();

  return isAnyInputChanged;
}

void Function::computeInputs() {
  for (auto const& f : inputFunctions) (*f)();
}

void Function::operator()() {
  if (!recompute) return;
  recompute = false;
  computeInputs();
  if (!areInputsDifferent()) return;
  updateSeenTicks();
  compute();
}

size_t Function::getInputTicks(size_t i) const{
  return inputs.at(i).seenTicks;
}

void Function::updateSeenTicks(){
  for(auto&x:inputs)
    x.seenTicks = x.resource->getTicks();
}
