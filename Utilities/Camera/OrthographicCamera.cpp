#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float bottom,
                                       float top, float left, float right) : Camera(eye, lookAt, up, near, far, bottom,
                                                                                    top, left, right) {

}

SquareMatrix OrthographicCamera::orthogonalProjection() {
   float invRL = 1/(right-left);
   float invTB = 1/(top-bottom);
   float invFN = 1/(far-near);
   return SquareMatrix(4, {2*invRL, 0, 0, -(right+left)*invRL,
                           0, 2*invTB, 0, -(top+bottom)*invTB,
                           0, 0, -2*invFN, -(far+near)*invFN,
                           0, 0, 0, 1});
}

SquareMatrix OrthographicCamera::onAxisOrthogonalProjection() {
   float invFN = 1/(far-near);
   return SquareMatrix(4, {1/right, 0, 0, 0,
                           0, 1/top, 0, 0,
                           0, 0, -2*invFN, -(far + near)*invFN,
                           0, 0, 0, 1});
}