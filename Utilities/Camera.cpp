#include "Camera.h"
#include "../Matrix/StandardMatrix.h"

Camera::Camera(Float3 eye, Float3 lookAt) : eye(std::move(eye)), lookAt(std::move(lookAt)), up(std::move(Float3(0, 0, 1))) {
   near = 0.1f;
   far = 100;
   viewAngle = degree2Radiants(45);
   right = near * tanf(viewAngle * 0.5f);

   yawAngle = acosf((lookAt - eye).getNormalized().getX());
}

Camera::Camera(Float3 eye, Float3 lookAt, Float3 up) : eye(std::move(eye)), lookAt(std::move(lookAt)), up(std::move(up)) {
   near = 0.1f;
   far = 100;
   viewAngle = degree2Radiants(45);
   right = near * tanf(viewAngle * 0.5f);

   yawAngle = acosf((lookAt - eye).getNormalized().getX());
}

Camera::Camera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float angle, float aspectRatio)
               : eye(std::move(eye)), lookAt(std::move(lookAt)), up(std::move(up)) {
   Camera::near = near;
   Camera::far = far;
   Camera::viewAngle = degree2Radiants(angle);
   Camera::aspectRatio = aspectRatio;

   Camera::right = near * tanf(Camera::viewAngle * 0.5f);
   Camera::top = aspectRatio * right;

   yawAngle = acosf((lookAt - eye).getNormalized().getX());
}

Camera::Camera(Float3 eye, Float3 lookAt, Float3 up, float near, float zoom, float far, float angle, float aspectRatio)
        : eye(std::move(eye)), lookAt(std::move(lookAt)), up(std::move(up)) {
   Camera::near = near + zoom;
   Camera::far = far;
   Camera::viewAngle = degree2Radiants(angle);
   Camera::aspectRatio = aspectRatio;

   Camera::right = near * tanf(Camera::viewAngle * 0.5f);
   //
   Camera::top = aspectRatio * right;

   yawAngle = acosf((lookAt - eye).getNormalized().getX());
}

Camera::Camera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float bottom, float top, float left,
               float right, float angle) : eye(std::move(eye)), lookAt(std::move(lookAt)), up(std::move(up)) {
   Camera::near = near;
   Camera::far = far;
   Camera::bottom = bottom;
   Camera::top = top;
   Camera::left = left;
   Camera::right = right;
   Camera::viewAngle = degree2Radiants(angle);

   yawAngle = acosf((lookAt - eye).getNormalized().getX());
}

SquareMatrix Camera::world2ViewMatrix() {
    /* Il problema della matrice non invertibile è risolvibile "spostando" la camera quando si vede che i prodotti vettoriali
     * che si usano per costruirla sono 0. Se considero una camera che guarda lungo z (negativi per esempio) allora xV=UPXzV = 0
     * (entrambi i vettori sono lungo Z globale)A questo punto si intercetta il prodotto vettoriale zero (o <eps) e si sposta
     * leggermente la camera (può essere utile anche considerando la camera che si muove). Praticamente si elimina la possibilità
     * che la camera vada in posizioni complicate
     */

   Float3 zView(std::move(eye - lookAt));
   zView.normalize();
   Float3 xView(std::move(up.crossProduct(zView)));

   if (xView.l2Norm() == 0) {
      xView = std::move(Float3(0.00015f, 1.00021f, 0.0f).crossProduct(zView));
   }

   xView.normalize();
   Float3 yView(std::move(zView.crossProduct(xView)));
   yView.normalize();

   SquareMatrix view2WorldMatrix(4, {xView.getX(), yView.getX(), zView.getX(), eye.getX(),
                                     xView.getY(), yView.getY(), zView.getY(), eye.getY(),
                                     xView.getZ(), yView.getZ(), zView.getZ(), eye.getZ(),
                                     0, 0, 0, 1});

   return SquareMatrix::calculateInverse(view2WorldMatrix);
}

SquareMatrix Camera::view2WorldMatrix() {
   Float3 zView(std::move(eye - lookAt));
   zView.normalize();
   Float3 xView(std::move(up.crossProduct(zView)));

   if (xView.l2Norm() == 0) {
      xView = std::move(Float3(0.00015f, 1.00021f, 0.0f).crossProduct(zView));
   }

   xView.normalize();
   Float3 yView(std::move(zView.crossProduct(xView)));
   yView.normalize();

   SquareMatrix view2WorldMatrix(4, {xView.getX(), yView.getX(), zView.getX(), eye.getX(),
                                     xView.getY(), yView.getY(), zView.getY(), eye.getY(),
                                     xView.getZ(), yView.getZ(), zView.getZ(), eye.getZ(),
                                     0, 0, 0, 1});

   return view2WorldMatrix;
}

float Camera::getNear() const {
   return near;
}

void Camera::setNear(float near) {
   Camera::near = near;
}

float Camera::getFar() const {
   return far;
}

void Camera::setFar(float far) {
   Camera::far = far;
}

float Camera::getLeft() const {
   return left;
}

void Camera::setLeft(float left) {
   Camera::left = left;
}

float Camera::getRight() const {
   return right;
}

void Camera::setRight(float right) {
   Camera::right = right;
}

float Camera::getBottom() const {
   return bottom;
}

void Camera::setBottom(float bottom) {
   Camera::bottom = bottom;
}

float Camera::getTop() const {
   return top;
}

void Camera::setTop(float top) {
   Camera::top = top;
}

float Camera::getAngle() const {
   return viewAngle;
}

void Camera::setAngle(float angle) {
   Camera::viewAngle = degree2Radiants(angle);
}

float Camera::getAspectRatio() const {
   return aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio) {
   Camera::aspectRatio = aspectRatio;

   top = aspectRatio * right;
}

const Float3 &Camera::getEye() const {
   return eye;
}

void Camera::setEye(const Float3 &eye) {
   Camera::eye = eye;
}

const Float3 &Camera::getLookAt() const {
   return lookAt;
}

void Camera::setLookAt(const Float3 &lookAt) {
   Camera::lookAt = lookAt;
}

const Float3 &Camera::getUp() const {
   return up;
}

void Camera::setUp(const Float3 &up) {
   Camera::up = up;
}

float Camera::getSensibility() const {
   return sensibility;
}

void Camera::setSensibility(float sensibility) {
   Camera::sensibility = sensibility;
}

float Camera::getYawAngle() const {
   return yawAngle;
}

void Camera::setYawAngle(float yawAngle) {
   Camera::yawAngle = yawAngle;
}
