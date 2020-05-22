#ifndef STANDARDMATRIX_H
#define STANDARDMATRIX_H

#include "../Vector/Float4.h"
#include "SquareMatrix.h"
#include <cmath>

//TODO implement in-class methods from this header

namespace Rotation {
   SquareMatrix rotationXAxisMatrix(const float& angleX);
   SquareMatrix rotationYAxisMatrix(const float& angleY);
   SquareMatrix rotationZAxisMatrix(const float& angleZ);
   SquareMatrix rotationByQuaternion(Float4 direction, const float& angle);

   Float3 axisXRotateVertex3(const Float3 &vector, const float& angleX);
   Float3 axisYRotateVertex3(const Float3 &vector, const float& angleY);
   Float3 axisZRotateVertex3(const Float3 &vector, const float& angleZ);

   Float4 quaternionAxisRotateVertex4(const Float4 &vector, Float4 &direction, const float &angle);
}

namespace Transform {
   SquareMatrix scaleTransform(const SquareMatrix &matrix, float scaleX, float scaleY, float scaleZ);
}

namespace Projection {
   //TODO check if move function into Camera
   SquareMatrix view2ClipProjection(const float& right, const float& left, const float& near, const float& far, const float& top,
                                    const float& bottom);

   SquareMatrix onAxisView2ClipProjection(const float& right, const float& top, const float& near, const float& far);

   SquareMatrix orthogonalProjection(const float& right, const float& left, const float& near, const float& far, const float& top,
                                     const float& bottom);

   SquareMatrix onAxisOrthogonalProjection(const float& right, const float& top, const float& near, const float& far);


}

static float degree2Radiants(float degree) {
   float inv = 1.0/180;

   return degree * inv * M_PI;
}

static float radiants2Degree(float radiants) {
   return radiants * 180 * M_1_PI;
}


#endif //STANDARDMATRIX_H
