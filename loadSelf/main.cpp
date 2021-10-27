/*

#include<iostream>
#include<dlfcn.h>

float add(float a,float b){
  return a+b;
}

int main(){
  //void*me = dlopen("/media/devel/git/prj/loadSelf/loadSelf",RTLD_LAZY);
  void*me = dlopen(NULL,RTLD_LAZY);

  if(!me){
    fprintf(stderr, "dlopen failed: %s\n", dlerror());
    return 0;
  }


  using ADD_TYPE = float(*)(float,float);
  ADD_TYPE loaded_add = (ADD_TYPE)dlsym(me,"add");

  std::cerr << loaded_add(3.5f,4.f) << std::endl;

  dlclose(me);
  return 0;
}

*/

  // file ds.c
  #include <stdio.h>
  #include <stdlib.h>
  #include <dlfcn.h>

  void hello ()
  {
    printf ("hello world\n");
  }

  using A = void(*)();
  int main (int argc, char **argv)
  {
    hello();
    char const*buf = "hello";
    void *hndl = dlopen (NULL, RTLD_LAZY);
    if (!hndl) { fprintf(stderr, "dlopen failed: %s\n", dlerror());
      exit (EXIT_FAILURE); };
    void (*fptr) (void) = (A)dlsym (hndl, buf);
    if (fptr != NULL)
      fptr ();
    else
      fprintf(stderr, "dlsym %s failed: %s\n", buf, dlerror());
    dlclose (hndl);
  }
