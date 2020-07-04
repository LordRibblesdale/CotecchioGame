#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
#include "../../Utilities/Camera.h"

class SpotLight : public Light {
   float maxAngle;   // Angolo di ombra
   float midAngle;   // Angolo di caratterizzazione comportamento

   Float3 direction;

   unsigned int frameBuffer;
   unsigned int depthMap;

public:
   SpotLight();
   SpotLight(Float3 origin, Float3 direction, Color color, float intensity, float midAngle_, float maxAngle_);

   SpotLight(SpotLight&& light);

   virtual float getIrradiance(float angle) override;

   unsigned int& getFrameBufferAsReference();

   unsigned int& getDepthMapAsReference();

   float getMaxAngle() const;

   void setMaxAngle(float maxAngle);

   float getMidAngle() const;

   void setMidAngle(float midAngle);

   const Float3 &getDirection() const;

   void setDirection(const Float3 &direction);
};

#endif //SPOTLIGHT_H
