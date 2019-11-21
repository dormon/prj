#pragma once

#include<Vars/Vars.h>
#include<Vars/Caller.h>
#include<Barrier.h>

#define FUNCTION_CALLER()\
  vars::Caller caller(vars,__FUNCTION__)

#define FUNCTION_PROLOGUE(object,...)\
  if(notChanged(vars,object,__FUNCTION__,{__VA_ARGS__}))return;\
  FUNCTION_CALLER()
