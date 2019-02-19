#pragma once

#include<cstdint>
#include<iostream>

template<typename T>
void sink(T const&);

template<>void sink<uint32_t   >(uint32_t    const&);
template<>void sink<uint64_t   >(uint64_t    const&);
template<>void sink<std::string>(std::string const&);
