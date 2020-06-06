#include <iostream>
#include "Camera.h"

Camera::Camera(Float3 eye, Float3 lookAt) : eye(std::move(eye)), lootAt(std::move(lookAt)),  up(std::move(Float3(0, 0, 1))) {}

Camera::Camera(Float3 eye, Float3 lookAt, Float3 up) : eye(std::move(eye)), lootAt(std::move(lookAt)),  up(std::move(up)) {}

SquareMatrix Camera::world2ViewMatrix() {
   /* TODO implement minimum variation
    * Il problema della matrice non invertibile è risolvibile "spostando" la camera quando si vee che i prodotti vettoriali
    * che si usano per costruirla sono 0. Se considero una camera che guarda lungo z (negativi per esempio) allora xV=UPXzV = 0
    * (entrambi i vettori sono lungo Z globale)A questo punto si intercetta il prodotto vettoriale zero (o <eps) e si sposta
    * leggermente la camera (può essere utile anche considerando la camera che si muove). Praticamente si elimina la possibilità
    * che la camera vada in posizioni complicate
    * TODO end implementation
    */
   Float3 zView(std::move(eye - lootAt));
   Float3 xView(std::move(up.crossProduct(zView)));
   Float3 yView(std::move(zView.crossProduct(xView)));

   xView.normalize();
   yView.normalize();
   zView.normalize();

   SquareMatrix view2WorldMatrix(4, {xView.getX(), yView.getX(), zView.getX(), eye.getX(),
                                     xView.getY(), yView.getY(), zView.getY(), eye.getY(),
                                     xView.getZ(), yView.getZ(), zView.getZ(), eye.getZ(),
                                     0, 0, 0, 1});

   return SquareMatrix::calculateInverse(view2WorldMatrix);
}