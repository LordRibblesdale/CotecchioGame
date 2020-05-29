#include <iostream>
#include "Camera.h"

bool test = false;

Camera::Camera(Float3 eye, Float3 lookAt) : eye(std::move(eye)), lootAt(std::move(lookAt)),  up(std::move(Float3(0, 0, 1))) {}

SquareMatrix Camera::world2ViewMatrix() {
   Float3 zView(std::move(eye - lootAt));
   Float3 xView(std::move(up.crossProduct(zView)));
   Float3 yView(std::move(zView.crossProduct(xView)));
   xView.normalize();
   yView.normalize();
   zView.normalize();

   SquareMatrix view2WorldMatrix(4, {xView.getX(), yView.getX(), zView.getX(), eye.getX(),
                                     xView.getY(), yView.getY(), zView.getY(), eye.getY(),
                                     xView.getZ(), yView.getY(), zView.getZ(), eye.getZ(),
                                     0, 0, 0, 1});

   return SquareMatrix::calculateInverse(view2WorldMatrix);
}
