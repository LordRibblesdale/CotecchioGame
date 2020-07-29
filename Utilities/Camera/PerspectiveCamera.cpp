#include "PerspectiveCamera.h"

#include "../../Matrix/StandardMatrix.h"

PerspectiveCamera::PerspectiveCamera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float angle, float aspectRatio)
      : Camera(std::move(eye), std::move(lookAt), std::move(up)) {
   Camera::near = near;
   Camera::far = far;
   PerspectiveCamera::viewAngle = degree2Radiants(angle);
   PerspectiveCamera::aspectRatio = aspectRatio;

   Camera::right = near * tanf(PerspectiveCamera::viewAngle * 0.5f);
   Camera::top = aspectRatio * right;
}

PerspectiveCamera::PerspectiveCamera(Float3 eye, Float3 lookAt, Float3 up, float near, float zoom, float far, float angle, float aspectRatio)
      : Camera(std::move(eye), std::move(lookAt), std::move(up)) {
   Camera::near = near + zoom;
   Camera::far = far;
   PerspectiveCamera::viewAngle = degree2Radiants(angle);
   PerspectiveCamera::aspectRatio = aspectRatio;

   Camera::right = near * tanf(PerspectiveCamera::viewAngle * 0.5f);

   Camera::top = aspectRatio * right;
}

PerspectiveCamera::PerspectiveCamera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float bottom, float top, float left,
               float right, float angle) : Camera(std::move(eye), std::move(lookAt), std::move(up)) {
   Camera::near = near;
   Camera::far = far;
   Camera::bottom = bottom;
   Camera::top = top;
   Camera::left = left;
   Camera::right = right;
   PerspectiveCamera::viewAngle = degree2Radiants(angle);
}

SquareMatrix PerspectiveCamera::view2ClipProjectiveMatrix() {
   float invRL = 1/(right-left);
   float invTB = 1/(top-bottom);
   float invFN = 1/(far-near);
   return SquareMatrix(4, {2*near*invRL, 0,            (right+left)*invRL, 0,
                           0,          2*near*invTB, (top+bottom)*invTB, 0,
                           0,          0,            -(far+near)*invFN,  -2*far*near*invFN,
                           0,          0,            -1,                 0});
}

void PerspectiveCamera::view2ClipProjectiveMatrix(SquareMatrix& matrix) {
   float invRL = 1/(right-left);
   float invTB = 1/(top-bottom);
   float invFN = 1/(far-near);
   matrix = std::move(SquareMatrix(4, {2*near*invRL, 0,            (right+left)*invRL, 0,
                                       0,          2*near*invTB, (top+bottom)*invTB, 0,
                                       0,          0,            -(far+near)*invFN,  -2*far*near*invFN,
                                       0,          0,            -1,                 0}));
}

SquareMatrix PerspectiveCamera::onAxisView2ClipProjectiveMatrix() {
   float invFN = 1/(far-near);
   return SquareMatrix(4, {near/right, 0, 0, 0,
                           0, near/top, 0, 0,
                           0, 0, -(far+near)*invFN, -2*far*near*invFN,
                           0, 0, -1,                0});
}

void PerspectiveCamera::onAxisView2ClipProjectiveMatrix(SquareMatrix& matrix) {
   float invFN = 1/(far-near);
   matrix = std::move(SquareMatrix(4, {near/right, 0, 0, 0,
                                       0, near/top, 0, 0,
                                       0, 0, -(far+near)*invFN, -2*far*near*invFN,
                                       0, 0, -1,                0}));
}

float PerspectiveCamera::getAngle() const {
   return viewAngle;
}

void PerspectiveCamera::setAngle(float angle) {
   PerspectiveCamera::viewAngle = degree2Radiants(angle);
}

float PerspectiveCamera::getAspectRatio() const {
   return aspectRatio;
}

void PerspectiveCamera::setAspectRatio(float aspectRatio) {
   PerspectiveCamera::aspectRatio = aspectRatio;

   top = aspectRatio * right;
}