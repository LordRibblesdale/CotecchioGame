#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

class SpotLight : Light {
   //TODO change variable names
   float maxAngle;   // Angolo di ombra
   float midAngle;   // Angolo di caratterizzazione comportamento

public:
   SpotLight();
   SpotLight(Float3 origin, Color color, float intensity, float midAngle_, float maxAngle_);

   virtual float getIrradiance(float angle) override;

};

#endif //SPOTLIGHT_H
