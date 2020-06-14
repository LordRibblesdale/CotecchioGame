#include "CameraRotation.h"
#include "../Matrix/StandardMatrix.h"
#include <cmath>

CameraRotation::CameraRotation(const Float3& initialPosition, const Float3& finalPosition, double duration, float radius) : quaternion(SquareMatrix(1, {})) {
   if (radius < 0) radius = -radius;

   Float3 middle(0.5f*initialPosition + 0.5f*finalPosition);
   angle = acosf((initialPosition - middle).dotProduct(finalPosition - middle));
   quaternion = std::move(Rotation::rotationByQuaternion(Float4(0, 0, 1, 0), angle/static_cast<float>(duration)));
}

Float4 CameraRotation::rotateCamera(const Float3 &position, double duration) {
   quaternion = std::move(Rotation::rotationByQuaternion(Float4(0, 0, 1, 0), angle/ static_cast<float>(duration)));

   // Automaticamente riconosce il costruttore con FloatVector
   return quaternion.multiplyVector(Float4(position, true));
}