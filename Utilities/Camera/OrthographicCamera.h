#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "Camera.h"

class OrthographicCamera : public Camera {
public:
   OrthographicCamera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float bottom, float top, float left, float right);

   SquareMatrix orthogonalProjection();
   SquareMatrix onAxisOrthogonalProjection();
};


#endif //ORTHOGRAPHICCAMERA_H
