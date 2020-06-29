#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
#include "../../Utilities/Camera.h"

class SpotLight : public Light {
   float maxAngle;   // Angolo di ombra
   float midAngle;   // Angolo di caratterizzazione comportamento

   Float3 direction;

   Camera camera;

public:
   SpotLight();
   SpotLight(Float3 origin, Float3 direction, Color color, float intensity, float midAngle_, float maxAngle_);

   virtual float getIrradiance(float angle) override;

};

#endif //SPOTLIGHT_H
