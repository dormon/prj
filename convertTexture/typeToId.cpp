#include"typeToId.h"

template<>unsigned typeToId<void              >(){return  0u;}
template<>unsigned typeToId<char              >(){return  1u;}
template<>unsigned typeToId<short             >(){return  2u;}
template<>unsigned typeToId<int               >(){return  3u;}
template<>unsigned typeToId<long long         >(){return  4u;}
template<>unsigned typeToId<unsigned char     >(){return  5u;}
template<>unsigned typeToId<unsigned short    >(){return  6u;}
template<>unsigned typeToId<unsigned int      >(){return  7u;}
template<>unsigned typeToId<unsigned long long>(){return  8u;}
template<>unsigned typeToId<float             >(){return  9u;}
template<>unsigned typeToId<double            >(){return 10u;}

template<>std::string typeToStr<void              >(){return "void"              ;}
template<>std::string typeToStr<char              >(){return "char"              ;}
template<>std::string typeToStr<short             >(){return "short"             ;}
template<>std::string typeToStr<int               >(){return "int"               ;}
template<>std::string typeToStr<long long         >(){return "long long"         ;}
template<>std::string typeToStr<unsigned char     >(){return "unsigned char"     ;}
template<>std::string typeToStr<unsigned short    >(){return "unsigned short"    ;}
template<>std::string typeToStr<unsigned int      >(){return "unsigned int"      ;}
template<>std::string typeToStr<unsigned long long>(){return "unsigned long long";}
template<>std::string typeToStr<float             >(){return "float"             ;}
template<>std::string typeToStr<double            >(){return "double"            ;}

