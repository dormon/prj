#pragma once

#include"lex2.h"

#include<iostream>

namespace ge{
  namespace util{
    template<typename T>
      class Range{
        protected:
          T _border[2];
        public:
          enum{
            START = 0,
            END   = 1,
          };
          enum AdjacencyType{
            INSIDE   = 0,
            LESS     = 1,
            GREATER  = 2,
            TOUCHING = 3,
          };
          static constexpr const unsigned bit[2] = {
            1u<<(0u                    ),
            1u<<(sizeof(unsigned)/2u*8u),
          };
          enum Adjacency{
            START_END_LESS             = (bit[START]<<LESS    )|(bit[END]<<LESS    ),
            START_LESS_END_TOUCHING    = (bit[START]<<LESS    )|(bit[END]<<TOUCHING),
            START_LESS_END_INSIDE      = (bit[START]<<LESS    )|(bit[END]<<INSIDE  ),
            START_LESS_END_GREATER     = (bit[START]<<LESS    )|(bit[END]<<GREATER ),
            START_END_INSIDE           = (bit[START]<<INSIDE  )|(bit[END]<<INSIDE  ),
            START_INSIDE_END_GREATER   = (bit[START]<<INSIDE  )|(bit[END]<<GREATER ),
            START_TOUCHING_END_GREATER = (bit[START]<<TOUCHING)|(bit[END]<<GREATER ),
            START_END_GREATER          = (bit[START]<<GREATER )|(bit[END]<<GREATER ),
          };
          Range(const T&start){
            this->set(START,start);
            this->set(END  ,start);
          }
          Range(const T&start,const T&end){
            this->set(START,start);
            this->set(END  ,end  );
          }
          inline T&get(unsigned i               ){return this->_border[i]       ;}
          inline T&set(unsigned i,const T&border){       this->_border[i]=border;}
          inline T    getStart(             ){return this->get(START      );}
          inline T    getEnd  (             ){return this->get(END        );}
          inline void setStart(const T&start){       this->set(START,start);}
          inline void setEnd  (const T&end  ){       this->set(END  ,end  );}
          Adjacency&getAdjacency(const Range<T>&other){
            int c[4];
            for(unsigned i=0;i<4;++i)
              c[i]=(int)(this->get(i/2)>other.get(i%2))-(int)(this->get(i/2)<other.get(i%2));
            unsigned result=0;
            for(unsigned i=0;i<4;++i)
              result+=bit[i/2]<<(unsigned(c[i]==-1+(i%2)*2)*(1+(i%2)));
            for(unsigned i=START;i<=END;++i)
              result+=(bit[i]<<TOUCHING)*((result&(bit[i]<<GREATER))&&this->get(i)==other.get(!i)+1-2*i);
            return result;
          }
          bool canBeConnected(const Range<T>&other){
            Adjacency adj=this->getAdjacency(other);
            if( !((adj&(bit[START]<<LESS   )&(bit[END]<<LESS   )) ||
                  (adj&(bit[START]<<GREATER)&(bit[END]<<GREATER)) ))return true;
            return false;
          }
      };
    class Range2{
      public:
        enum AdjacencyType{
          LESS     = 0,
          TOUCHING = 1,
          INSIDE   = 2,
          GREATER  = 3,
        };
        static constexpr const unsigned startBitPosition = 0                   ;
        static constexpr const unsigned endBitPosition   = sizeof(unsigned)/2*8;
        enum Adjacency{
          START_END_LESS             = ((1<<startBitPosition)<<LESS    )|((1<<endBitPosition)<<LESS    ),
          START_LESS_END_TOUCHING    = ((1<<startBitPosition)<<LESS    )|((1<<endBitPosition)<<TOUCHING),
          START_LESS_END_INSIDE      = ((1<<startBitPosition)<<LESS    )|((1<<endBitPosition)<<INSIDE  ),
          START_LESS_END_GREATER     = ((1<<startBitPosition)<<LESS    )|((1<<endBitPosition)<<GREATER ),
          START_END_INSIDE           = ((1<<startBitPosition)<<INSIDE  )|((1<<endBitPosition)<<INSIDE  ),
          START_INSIDE_END_GREATER   = ((1<<startBitPosition)<<INSIDE  )|((1<<endBitPosition)<<GREATER ),
          START_TOUCHING_END_GREATER = ((1<<startBitPosition)<<TOUCHING)|((1<<endBitPosition)<<GREATER ),
          START_END_GREATER          = ((1<<startBitPosition)<<GREATER )|((1<<endBitPosition)<<GREATER ),
        };
      private:
        Lex2 _start;
        Lex2 _end  ;
      public:
        Range2(Lex2 start = Lex2(),Lex2 end = Lex2());
        Range2(Range2&a,Range2&b);
        bool inside(const void*data,unsigned size,unsigned offset);
        Lex2&getStart();
        Lex2&getEnd  ();
        void setStart(Lex2 start = Lex2());
        void setEnd  (Lex2 end   = Lex2());
        void set     (Lex2 start = Lex2(),Lex2 end = Lex2());
        Adjacency getAdjacency(Range2&b);
        bool      canBeConnected(Range2&b);
        std::string toStr();

    };

    class ByteRange2{
      protected:
        unsigned char _start;
        unsigned char _end  ;
      public:
        ByteRange2(unsigned char start);
        ByteRange2(unsigned char start,unsigned end);
        unsigned char getStart();
        unsigned char getEnd  ();
        void          setStart(unsigned char start);
        void          setEnd  (unsigned char end  );

    };
  }
}
