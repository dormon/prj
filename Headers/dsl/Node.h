#pragma once

namespace dsl{

  template<template<typename...>class CONTAINER,template<typename...>class DECORATOR>
  class Node{
    public:
      CONTAINER< typename DECORATOR< Node<CONTAINER,DECORATOR> >::type >childs;
  };

}
