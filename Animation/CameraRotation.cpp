#include "CameraRotation.h"
#include "../Matrix/StandardMatrix.h"
#include <cmath>

CameraRotation::CameraRotation(const Float3& initialPosition, const Float3& finalPosition, float radius) : quaternion(SquareMatrix(1, {})) {
   if (radius < 0) radius = -radius;

   center = std::move(Float3(0.5f * initialPosition + 0.5f * finalPosition));

   Float3 tmp1(initialPosition - center);
   Float3 tmp2(finalPosition - center);

   tmp1.normalize();
   tmp2.normalize();

   float dot = tmp1.dotProduct(tmp2);
   dot = dot < -1 ? -1 : dot;
   dot = dot > 1 ? 1 : dot;

   angle = acosf(dot);
}

Float3 CameraRotation::rotateCamera(const Float3 &position, double duration) {
   quaternion = std::move(Rotation::rotationZAxisMatrix(angle* static_cast<float>(duration)));

   Float3 tmp(position - center);
   tmp = std::move(quaternion.multiplyVector(tmp));
   return tmp + center;

   /*
   Float4 tmp(position - center, true);
   return Rotation::quaternionAxisRotateVertex4(tmp, Float4(0, 0, 1, 0), angle* static_cast<float>(duration)).getFloat3() + center;
    */
}