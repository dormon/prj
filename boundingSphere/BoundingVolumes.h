#pragma once
//#include <geSG/Export.h>
#include <glm/glm.hpp>
#include <memory>

namespace ge
{
   namespace sg
   {
      class BoundingVolume
      {};

      class BoundingSphere : public BoundingVolume
      {
      public:
          BoundingSphere()
             : center(glm::vec3())
             , radius(0.0f)
          {}

          BoundingSphere( const BoundingSphere& other)
             : center(other.center)
             , radius(other.radius)
          {}

          /*void expand(const BoundingSphere& bs)
          {
             glm::vec3 newCenter = (bs.center + this->center) / 2.0f;
             float rad1 = glm::distance(newCenter, this->center) + this->radius;
             float rad2 = glm::distance(newCenter, bs.center) + bs.radius;

             this->center = newCenter;
             this->radius = rad1 > rad2 ? rad1 : rad2;
          }*/

          /**
           * 
           */
          template<template <typename...> class BSContainer >
          void expand(const BSContainer<std::shared_ptr<BoundingSphere>>& container)
          {
             glm::vec3 newCenter = this->center;
             float newRadius = 0.0f;

             //compute new center
             for(auto BS : container)
             {
                newCenter += (*BS).center;
             }

             newCenter /= container.size()+1;
             newRadius = glm::distance(newCenter, this->center) + this->radius;

             for(auto BS : container)
             {
                float rad = glm::distance(newCenter, BS->center) + BS->radius;
                this->radius = newRadius >= rad ? newRadius : rad;
             }
          }


          glm::vec3 center;
          float radius;
      };

      template<typename Insides>
      class BoundingSphereTemplate : public BoundingSphere
      {
      public:
         BoundingSphereTemplate()
            : BoundingSphere()
         {}

         BoundingSphereTemplate(std::shared_ptr<Insides> & insides)
            : BoundingSphere()
            , insideThing(insides)
         {}

         BoundingSphereTemplate(const BoundingSphereTemplate& other)
            : BoundingSphere(other)
            , insideThing(other.insideThing)
         {}

         std::shared_ptr<Insides> insideThing;
      };
   }
}
