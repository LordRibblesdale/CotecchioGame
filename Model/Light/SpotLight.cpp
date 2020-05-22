#include "SpotLight.h"

SpotLight::SpotLight() : Light(std::move(Float3(0, 0, 0)), 10) {
   midAngle = degree2Radiants(35);
   maxAngle = degree2Radiants(45);
}

SpotLight::SpotLight(Float3 origin, float intensity, float midAngle_, float maxAngle_) : Light(std::move(origin), intensity) {
   midAngle = midAngle_;
   maxAngle = maxAngle_;
}

float SpotLight::getIrradiance(float angle) {
   float falloff;
   if (angle > maxAngle) {
      falloff = 0;
   } else if (angle <= maxAngle && angle > midAngle) {
      //TODO optimise cos usage
      float cosMidAngle = cosf(midAngle);
      float num = cosf(angle) - cosMidAngle;
      float den = 1/(cosf(maxAngle) - cosMidAngle);

      falloff = num * den;
   } else {
      falloff = 1;
   }

   return intensity * falloff;
}
