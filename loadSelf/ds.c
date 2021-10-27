// file ds.c
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void hello ()
{
  printf ("hello world\n");
}

int main (int argc, char **argv)
{
  char *buf = "hello";
  void *hndl = dlopen (NULL, RTLD_LAZY);
  if (!hndl) { 
    fprintf(stderr, "dlopen failed: %s\n", dlerror()); 
    exit(EXIT_FAILURE); 
  };
  void (*fptr) (void) = dlsym (hndl, buf);

  if (fptr == NULL){
    fprintf(stderr, "dlsym %s failed: %s\n", buf, dlerror());
    exit(EXIT_FAILURE);
  }
  
  fptr ();
  dlclose (hndl);
  return 0;
}
