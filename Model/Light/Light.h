#ifndef LIGHT_H
#define LIGHT_H

#include <utility>
#include <cmath>

#include "../../Vector/Float3.h"
#include "../../Utilities/Color.h"

class Light {
protected:
   Float3 origin;
   Color color;
   float intensity;

   Light(Float3 o, Color c, float i) : origin(std::move(o)), intensity(i), color(std::move(c)) {}

   static float degree2Radiants(float degree) {
      float inv = 1.0/180;

      return degree * inv * M_PI;
   }

   static float radiants2Degree(float radiants) {
      return radiants * 180 * M_1_PI;
   }
public:
   virtual float getIrradiance(float distance) = 0;

   // TODO check function call if inheritance is private
   inline const Color& getColor() {
      return color;
   }
};

#endif //LIGHT_H
