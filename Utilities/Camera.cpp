#include "Camera.h"

Camera::Camera(Float3 eye, Float3 lookAt) : eye(std::move(eye)), lootAt(std::move(lookAt)),  up(std::move(Float3(0, 0, 1))) {}

SquareMatrix Camera::world2ViewMatrix() {
   Float3 zView(eye - lootAt);
   Float3 xView(std::move(up.crossProduct(zView)));
   Float3 yView(std::move(zView.crossProduct(xView)));
   xView.normalize();
   yView.normalize();
   zView.normalize();

   SquareMatrix view2WorldMatrix(4, {});

   for (int i = 0; i < xView.getSize(); ++i) {
      view2WorldMatrix.getArray()[i * view2WorldMatrix.getDimension()] = xView.getVector().get()[i];
      view2WorldMatrix.getArray()[i * view2WorldMatrix.getDimension() + 1] = yView.getVector().get()[i];
      view2WorldMatrix.getArray()[i * view2WorldMatrix.getDimension() + 2] = zView.getVector().get()[i];
   }

   view2WorldMatrix.getArray()[view2WorldMatrix.getDimension() * view2WorldMatrix.getDimension() - 1] = 1;

   return SquareMatrix::calculateInverse(view2WorldMatrix);
}
