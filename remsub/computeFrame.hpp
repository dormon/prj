#pragma once

#include<Vars/Fwd.h>
#include <VarsGLMDecorator/VarsGLMDecorator.h>

using DVars = VarsGLMDecorator<vars::Vars>;
void computeFrame(DVars&vars);
