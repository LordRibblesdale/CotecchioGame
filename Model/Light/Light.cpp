#include "Light.h"

unsigned int& Light::getFrameBufferAsReference() {
   return frameBuffer;
}

unsigned int& Light::getDepthMapAsReference() {
   return depthMap;
}

float Light::degree2Radiants(float degree) {
   float inv = 1.0f/180.0f;

   return degree * inv * M_PI;
}

float Light::radiants2Degree(float radiants) {
   return radiants * 180 * M_1_PI;
}

const Color &Light::getColor() const {
   return color;
}

const Float3 &Light::getOrigin() const {
   return origin;
}

void Light::setOrigin(const Float3 &origin) {
   Light::origin = origin;
}

void Light::setColor(const Color &color) {
   Light::color = color;
}

float Light::getIntensity() const {
   return intensity;
}

void Light::setIntensity(float intensity) {
   Light::intensity = intensity;
}
