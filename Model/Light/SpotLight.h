#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
#include "../../Utilities/Camera/PerspectiveCamera.h"

class SpotLight : public Light {
   float maxAngle;   // Angolo di ombra
   float midAngle;   // Angolo di caratterizzazione comportamento

   Float3 direction;

   std::unique_ptr<PerspectiveCamera> camera;
public:
   SpotLight();
   SpotLight(const Float3& origin, const Float3& lookAt, const Color& color1, float intensity, float midAngle_, float maxAngle_);

   SpotLight(SpotLight&& light);

   float getIrradiance(float angle) override;

   std::unique_ptr<PerspectiveCamera>& getCamera();

   virtual void setOrigin(const Float3 &origin) override;

   float getMaxAngle() const;
   void setMaxAngle(float maxAngle);

   float getMidAngle() const;
   void setMidAngle(float midAngle);

   const Float3& getDirection() const;
   void setDirection(const Float3& direction);
};

#endif //SPOTLIGHT_H
