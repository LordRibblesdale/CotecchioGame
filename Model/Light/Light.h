#ifndef LIGHT_H
#define LIGHT_H

#include <utility>
#include <cmath>

#include "../../Vector/Float3.h"

class Light {
protected:
   Float3 origin;
   float intensity;

   Light(Float3 o, float i) : origin(std::move(o)), intensity(i) {}

   static float degree2Radiants(float degree) {
      float inv = 1.0/180;

      return degree * inv * M_PI;
   }

   static float radiants2Degree(float radiants) {
      return radiants * 180 * M_1_PI;
   }
public:
   virtual float getIrradiance(float distance) = 0;
};

#endif //LIGHT_H
