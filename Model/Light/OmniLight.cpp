#include "OmniLight.h"

#include <utility>

OmniLight::OmniLight() : Light(std::move(Float3(0, 0, 0)), std::move(Color(1)), 10) {
   nearFalloffDistance = intensity; // As distance in meters
   farFalloffDistance = 1.5f*intensity;
}

OmniLight::OmniLight(Float3 origin, Color color, float intensity) : Light(std::move(origin), std::move(color), intensity) {
   nearFalloffDistance = intensity; // As distance in meters
   farFalloffDistance = 1.5f*intensity;
}

float OmniLight::getIrradiance(float distance) {
   float falloff;
   if (distance < nearFalloffDistance) {
      falloff = 1;
   } else if (distance >= nearFalloffDistance && distance < farFalloffDistance) {
      float num = farFalloffDistance - distance;
      float den = 1 / (farFalloffDistance - nearFalloffDistance);

      falloff = num*den;
   } else {
      falloff = 0;
   }

   return intensity * falloff;
}
