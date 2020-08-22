#pragma once

#include<SDL_events.h>
#include<Vars/Fwd.h>

void onKey(vars::Vars&vars,SDL_Event const&event,bool DOWN);
