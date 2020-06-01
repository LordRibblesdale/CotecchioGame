#ifndef OMNILIGHT_H
#define OMNILIGHT_H

#include "Light.h"

class OmniLight : Light {
   float nearFalloffDistance;
   float farFalloffDistance;

public:
   OmniLight();
   OmniLight(Float3 origin, Color c, float intensity);

   virtual float getIrradiance(float distance) override;
};


#endif //OMNILIGHT_H
