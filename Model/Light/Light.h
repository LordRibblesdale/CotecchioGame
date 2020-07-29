#ifndef LIGHT_H
#define LIGHT_H

#include <cmath>

#include "../../Vector/Float3.h"
#include "../../Utilities/Color.h"
#include "../../Utilities/Camera/Camera.h"

class Light {
protected:
   Float3 origin;
   Color color;
   float intensity;

   unsigned int frameBuffer;
   unsigned int depthMap;

   Light(Float3 o, Color c, float i) : origin(std::move(o)), intensity(i), color(std::move(c)) {}

   static float degree2Radiants(float degree);

   static float radiants2Degree(float radiants);

public:
   virtual float getIrradiance(float distance) = 0;

   unsigned int& getFrameBufferAsReference();

   unsigned int& getDepthMapAsReference();

   const Color & getColor() const;
   void setColor(const Color &color);

   const Float3 &getOrigin() const;
   virtual void setOrigin(const Float3 &origin);

   float getIntensity() const;
   void setIntensity(float intensity);
};

#endif //LIGHT_H
