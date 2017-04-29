#include"registrationTest.h"

#include"arrayRegistrationTest.h"
#include"structRegistrationTest.h"
#include<geCore/TypeRegister.h>

using namespace ge::core;



void registrationTest(){
  arrayRegistrationTest();
  structRegistrationTest();
}
