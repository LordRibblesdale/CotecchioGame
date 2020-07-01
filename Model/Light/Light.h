#ifndef LIGHT_H
#define LIGHT_H

#include <utility>
#include <cmath>

#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

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

   const Color& getColor() {
      return color;
   }

   const Float3 &getOrigin() const {
      return origin;
   }

   void setOrigin(const Float3 &origin) {
      Light::origin = origin;
   }

   void setColor(const Color &color) {
      Light::color = color;
   }

   float getIntensity() const {
      return intensity;
   }

   void setIntensity(float intensity) {
      Light::intensity = intensity;
   }
};

#endif //LIGHT_H
