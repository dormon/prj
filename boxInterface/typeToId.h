#pragma once

#include<iostream>

template<typename TYPE>
unsigned typeToId();

template<>unsigned typeToId<void              >();
template<>unsigned typeToId<char              >();
template<>unsigned typeToId<short             >();
template<>unsigned typeToId<int               >();
template<>unsigned typeToId<long long         >();
template<>unsigned typeToId<unsigned char     >();
template<>unsigned typeToId<unsigned short    >();
template<>unsigned typeToId<unsigned int      >();
template<>unsigned typeToId<unsigned long long>();
template<>unsigned typeToId<float             >();
template<>unsigned typeToId<double            >();

template<typename TYPE>
std::string typeToStr();

template<>std::string typeToStr<void              >();
template<>std::string typeToStr<char              >();
template<>std::string typeToStr<short             >();
template<>std::string typeToStr<int               >();
template<>std::string typeToStr<long long         >();
template<>std::string typeToStr<unsigned char     >();
template<>std::string typeToStr<unsigned short    >();
template<>std::string typeToStr<unsigned int      >();
template<>std::string typeToStr<unsigned long long>();
template<>std::string typeToStr<float             >();
template<>std::string typeToStr<double            >();

