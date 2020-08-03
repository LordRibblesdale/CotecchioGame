#include "CameraTranslation.h"

CameraTranslation::CameraTranslation(const Float3& initialPosition, const Float3& finalPosition) : initialPosition(initialPosition), finalPosition(finalPosition) {}

Float3 CameraTranslation::translateCamera(double time, double duration) {
   double t = time / duration;

   return (1-t)* initialPosition + t*finalPosition;
}