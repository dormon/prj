#pragma once

template<template<typename...>class CONTAINER,typename TYPE>
class ContainerOfShared: public CONTAINER<TYPE>{
};

template<typename TYPE>
class PointerDecorator{
  public:
    using type = TYPE*;
};

template<template<typename...>class CONTAINER>
class Node: public CONTAINER<Node<CONTAINER>>{
};
