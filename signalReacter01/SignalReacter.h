#pragma once

#include<set>
#include<array>
#include<map>
#include<vector>
#include<sstream>

template<typename SIGNAL_TYPE>
class SignalReacter{
  public:
    virtual ~SignalReacter();
    virtual void react(SIGNAL_TYPE const&) = 0;
    void emit (SIGNAL_TYPE const&signal);
    void addTarget(SignalReacter*const&target);
    void addSource(SignalReacter*const&source);
    void removeTarget(SignalReacter*const&target);
    void removeSource(SignalReacter*const&target);
    std::set<SignalReacter*>const&getTargets()const;
    std::set<SignalReacter*>const&getSources()const;
  protected:
    std::array<std::set<SignalReacter*>,2>signalNeighbors;
    enum NeighborType{
      SOURCES = 0,
      TARGETS = 1,
    };

    void addNeighbor(SignalReacter*const&reacter,NeighborType type);

    void removeNeighbor(SignalReacter*const&reacter,NeighborType type);

    void eraseNeighbor(SignalReacter*const&reacter,NeighborType type);

    void removeNeighbors(NeighborType type);

    NeighborType getOppositeType(NeighborType t);
};


template<typename SIGNAL_TYPE>
SignalReacter<SIGNAL_TYPE>::~SignalReacter(){
  removeNeighbors(SOURCES);
  removeNeighbors(TARGETS);
}

template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::emit (SIGNAL_TYPE const&signal){
  for(auto const&target:signalNeighbors.at(TARGETS))
    target->react(signal);
}

template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::addTarget(SignalReacter*const&target){
  addNeighbor(target,TARGETS);
}

template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::addSource(SignalReacter*const&source){
  addNeighbor(source,SOURCES);
}

template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::removeTarget(SignalReacter*const&target){
  removeNeighbor(target,TARGETS);
}

template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::removeSource(SignalReacter*const&target){
  removeNeighbor(target,SOURCES);
}

template<typename SIGNAL_TYPE>
std::set<SignalReacter<SIGNAL_TYPE>*>const&SignalReacter<SIGNAL_TYPE>::getTargets()const{
  return signalNeighbors.at(TARGETS);
}

template<typename SIGNAL_TYPE>
std::set<SignalReacter<SIGNAL_TYPE>*>const&SignalReacter<SIGNAL_TYPE>::getSources()const{
  return signalNeighbors.at(SOURCES);
}



template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::addNeighbor(SignalReacter*const&reacter,NeighborType type){
  auto const oppositeType = getOppositeType(type);

  signalNeighbors.at(type).insert(reacter);
  reacter->signalNeighbors.at(oppositeType).insert(this);
}

template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::removeNeighbor(SignalReacter*const&reacter,NeighborType type){
  eraseNeighbor(reacter,type);
  reacter->eraseNeighbor(this,getOppositeType(type));
}

template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::eraseNeighbor(SignalReacter*const&reacter,NeighborType type){
  auto const it = signalNeighbors.at(type).find(reacter);
  if(it != std::cend(signalNeighbors.at(type)))
    signalNeighbors.at(type).erase(it);
}

template<typename SIGNAL_TYPE>
void SignalReacter<SIGNAL_TYPE>::removeNeighbors(NeighborType type){
  std::vector<SignalReacter*>allReacters;
  for(auto const&neighbor:signalNeighbors.at(type))
    allReacters.push_back(neighbor);
  for(auto const&reacter:allReacters)
    removeNeighbor(reacter,type);
}

template<typename SIGNAL_TYPE>
typename SignalReacter<SIGNAL_TYPE>::NeighborType SignalReacter<SIGNAL_TYPE>::getOppositeType(NeighborType t){
  if(t == SOURCES)return TARGETS;
  return SOURCES;
}

