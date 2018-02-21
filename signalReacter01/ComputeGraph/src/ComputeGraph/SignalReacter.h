#pragma once

#include <array>
#include <map>
#include <set>
#include <sstream>
#include <vector>

template <typename SIGNAL_TYPE>
class SignalReacter {
  public:
  virtual ~SignalReacter();
  virtual void react(SIGNAL_TYPE const &) = 0;
  void emit(SIGNAL_TYPE const &signal);
  void addTarget(SignalReacter *const &target);
  void addSource(SignalReacter *const &source);
  void removeTarget(SignalReacter *const &target);
  void removeSource(SignalReacter *const &target);

  using Neighbors = std::map<SignalReacter *, size_t>;
  Neighbors const &getTargets() const;
  Neighbors const &getSources() const;

  protected:
  enum NeighborParts {
    REACTER = 0,
    COUNTER = 1,
  };
  std::array<Neighbors, 2> signalNeighbors;
  enum NeighborType {
    SOURCES = 0,
    TARGETS = 1,
  };

  void addNeighbor(SignalReacter *const &reacter, NeighborType type);

  void addNeighborOrIncrementCounter(Neighbors &neighbors, SignalReacter *r);
  void removeNeighborOrDecrementCounter(Neighbors &neighbors, SignalReacter *r);

  void removeNeighbor(SignalReacter *const &reacter, NeighborType type);

  void eraseNeighbor(SignalReacter *const &reacter, NeighborType type);

  void eraseThisFromNeighbors(NeighborType type);

  NeighborType getOppositeType(NeighborType t);
};

template <typename SIGNAL_TYPE>
SignalReacter<SIGNAL_TYPE>::~SignalReacter() {
  eraseThisFromNeighbors(SOURCES);
  eraseThisFromNeighbors(TARGETS);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::eraseNeighbor(SignalReacter *const &reacter,
                                               NeighborType          type) {
  auto const it = signalNeighbors.at(type).find(reacter);
  if (it != std::cend(signalNeighbors.at(type)))
    signalNeighbors.at(type).erase(it);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::eraseThisFromNeighbors(NeighborType type) {
  std::vector<SignalReacter *> allReacters;
  for (auto const &neighbor : signalNeighbors.at(type))
    allReacters.push_back(neighbor.first);
  for (auto const &reacter : allReacters)
    reacter->eraseNeighbor(this, getOppositeType(type));
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::emit(SIGNAL_TYPE const &signal) {
  for (auto const &target : signalNeighbors.at(TARGETS))
    target.first->react(signal);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::addTarget(SignalReacter *const &target) {
  addNeighbor(target, TARGETS);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::addSource(SignalReacter *const &source) {
  addNeighbor(source, SOURCES);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::removeTarget(SignalReacter *const &target) {
  removeNeighbor(target, TARGETS);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::removeSource(SignalReacter *const &target) {
  removeNeighbor(target, SOURCES);
}

template <typename SIGNAL_TYPE>
typename SignalReacter<SIGNAL_TYPE>::Neighbors const &
SignalReacter<SIGNAL_TYPE>::getTargets() const {
  return signalNeighbors.at(TARGETS);
}

template <typename SIGNAL_TYPE>
typename SignalReacter<SIGNAL_TYPE>::Neighbors const &
SignalReacter<SIGNAL_TYPE>::getSources() const {
  return signalNeighbors.at(SOURCES);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::addNeighborOrIncrementCounter(
    Neighbors &neighbors, SignalReacter *r) {
  auto it = neighbors.find(r);
  if (it != neighbors.end())
    it->second++;
  else
    neighbors[r] = 1;
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::addNeighbor(SignalReacter *const &reacter,
                                             NeighborType          type) {
  if (reacter == nullptr) return;

  auto &neighbors = signalNeighbors.at(type);
  addNeighborOrIncrementCounter(neighbors, reacter);

  auto const oppositeType     = getOppositeType(type);
  auto &     reacterNeighbors = reacter->signalNeighbors.at(oppositeType);
  addNeighborOrIncrementCounter(reacterNeighbors, this);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::removeNeighborOrDecrementCounter(
    Neighbors &neighbors, SignalReacter *r) {
  auto it = neighbors.find(r);
  if (it == neighbors.end()) return;
  it->second--;
  if (it->second == 0) neighbors.erase(it);
}

template <typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::removeNeighbor(SignalReacter *const &reacter,
                                                NeighborType          type) {
  if (reacter == nullptr) return;

  auto &neighbors = signalNeighbors.at(type);
  removeNeighborOrDecrementCounter(neighbors, reacter);

  auto const oppositeType     = getOppositeType(type);
  auto &     reacterNeighbors = reacter->signalNeighbors.at(oppositeType);
  removeNeighborOrDecrementCounter(reacterNeighbors, this);
}

template <typename SIGNAL_TYPE>
typename SignalReacter<SIGNAL_TYPE>::NeighborType
SignalReacter<SIGNAL_TYPE>::getOppositeType(NeighborType t) {
  if (t == SOURCES) return TARGETS;
  return SOURCES;
}
