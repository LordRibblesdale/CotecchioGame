#ifndef STANDARDMATRIX_H
#define STANDARDMATRIX_H

#include "../Vector/Float4.h"
#include "SquareMatrix.h"
#include <cmath>

//TODO add Identity and Translation

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
   SquareMatrix scaleMatrix4(float scaleX, float scaleY, float scaleZ);
   SquareMatrix scaleTransform3(const SquareMatrix &matrix, float scaleX, float scaleY, float scaleZ);

   SquareMatrix translateMatrix4(float x, float y, float z);

   SquareMatrix tranScalaRotoMatrix4(float xTransl, float yTransl, float zTransl, float xScale, float yScale, float zScale);
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
   float inv = 1.0f/180;

   return degree * inv * M_PI;
}

static float radiants2Degree(float radiants) {
   return radiants * 180.0f * M_1_PI;
}


#endif //STANDARDMATRIX_H
