#include<SharedLibraryOnStaticLibraries_A.h>
#include<SharedLibraryOnStaticLibraries_B.h>

float fce(float a){
  return internalA(a)+internalB(a);
}
