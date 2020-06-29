#include "SpotLight.h"

SpotLight::SpotLight() : Light(std::move(Float3(0, 0, 0)), std::move(Color(1)), 10),
         camera(std::move(Camera(origin, Float3(0, 0, 0), Float3(0, 0, 1), 0.1f, 1000, 90, 1))){
   midAngle = degree2Radiants(35);
   maxAngle = degree2Radiants(45);
}

SpotLight::SpotLight(Float3 origin, Float3 direction, Color color1, float intensity, float midAngle_, float maxAngle_) : Light(std::move(origin), std::move(color1), intensity),
         camera(std::move(Camera(origin, direction, Float3(0, 0, 1), 0.1f, 1000, 90, 1))){
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
