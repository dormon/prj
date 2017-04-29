#pragma once

template<typename TYPE>
void*toVoidPtr(TYPE&t){return (void*)&t;}

template<typename TYPE>
void*toVoidPtr(TYPE*t){return (void*)t;}

template<typename TYPE>
TYPE fromVoidPtr(void*data){return *((TYPE*)data);}


