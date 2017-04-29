#pragma once

template<template<typename...>class CONTAINER,typename TYPE>
class ContainerOfShared: public CONTAINER<std::shared_ptr<TYPE>>{
};

