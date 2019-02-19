#include"sink.h"

template<>void sink<uint32_t   >(uint32_t    const&){}
template<>void sink<uint64_t   >(uint64_t    const&){}
template<>void sink<std::string>(std::string const&){}
