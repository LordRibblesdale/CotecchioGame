#include <iostream>
#include "SpotLight.h"

SpotLight::SpotLight() : Light(std::move(Float3(0, 0, 0)), std::move(Color(1)), 10) {
   camera = std::move(std::make_unique<Camera>(origin, Float3(0, 0, 0), Float3(0, 0, 1), 0.1f, 1000, 90, 1));

   midAngle = degree2Radiants(35);
   maxAngle = degree2Radiants(45);
}

SpotLight::SpotLight(const Float3& origin, const Float3& lookAt, const Color& color1, float intensity, float midAngle_, float maxAngle_) : Light(origin, color1, intensity) {
   // Low-perspective aliasing
   camera = std::move(std::make_unique<Camera>(origin, lookAt, Float3(0, 0, 1), 2.5f, 100, -3, 3, -3, 3, maxAngle_));

   midAngle = midAngle_;
   maxAngle = maxAngle_;
}

SpotLight::SpotLight(SpotLight&& light) : Light(light.getOrigin(), light.getColor(), light.getIntensity()) {
   direction = light.getDirection();
   midAngle = light.getMidAngle();
   maxAngle = light.getMaxAngle();

   camera = std::move(light.camera);
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

float SpotLight::getMaxAngle() const {
   return maxAngle;
}

void SpotLight::setMaxAngle(float maxAngle) {
   SpotLight::maxAngle = maxAngle;
}

float SpotLight::getMidAngle() const {
   return midAngle;
}

void SpotLight::setMidAngle(float midAngle) {
   SpotLight::midAngle = midAngle;
}

const Float3 &SpotLight::getDirection() const {
   return direction;
}

void SpotLight::setDirection(const Float3& direction) {
   if (camera) {
      camera->setLookAt(direction);
   }

   SpotLight::direction = direction;
}
