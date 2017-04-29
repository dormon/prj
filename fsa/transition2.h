#pragma once

#include"lex2.h"
#include"range2.h"
#include"state2.h"

#include<iostream>


namespace ge{
  namespace util{
    class FSA2;
    typedef void(*RuleCallback2)(FSA2*,void*);
    class Transition2{
      private:
        State2*       _next    ;
        RuleCallback2 _callback;
        void*         _data    ;
        Range2        _range   ;
      public:
        Transition2(
            State2*       next               ,
            Range2        range    = Range2(),
            RuleCallback2 callback = NULL    ,
            void*         data     = NULL    );
        bool inside(const void*data,unsigned size,unsigned offset);
        void call(FSA2*fsa);
        State2*       getNext    ();
        RuleCallback2 getCallback();
        void*         getData    ();
        Range2&       getRange   ();
        void          setNext    (State2*state);
        void          setCallback(RuleCallback2 callback = NULL,void*data = NULL);
        bool          canBeJoined(Transition2&b);
    };

  }
}
