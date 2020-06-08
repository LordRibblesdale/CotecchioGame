#ifndef STANDARDMATRIX_H
#define STANDARDMATRIX_H

#include "../Vector/Float4.h"
#include "SquareMatrix.h"
#include "../Utilities/Camera.h"
#include <cmath>

#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.69314718055994530942
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

//TODO add Identity and Translation

namespace Rotation {
   SquareMatrix rotationXAxisMatrix(const float& angleX);
   SquareMatrix rotationYAxisMatrix(const float& angleY);
   SquareMatrix rotationZAxisMatrix(const float& angleZ);
   SquareMatrix rotationByQuaternion(const Float4& direction, const float& angle);

   Float3 axisXRotateVertex3(const Float3 &vector, const float& angleX);
   Float3 axisYRotateVertex3(const Float3 &vector, const float& angleY);
   Float3 axisZRotateVertex3(const Float3 &vector, const float& angleZ);

   Float4 quaternionAxisRotateVertex4(const Float4 &vector, const Float4 &direction, const float &angle);
}

namespace Transform {
   SquareMatrix scaleMatrix4(float scaleX, float scaleY, float scaleZ);
   SquareMatrix scaleTransform3(const SquareMatrix &matrix, float scaleX, float scaleY, float scaleZ);

   SquareMatrix translateMatrix4(float x, float y, float z);

   SquareMatrix tranScalaRotoMatrix4(float xTransl, float yTransl, float zTransl, float xScale, float yScale, float zScale, float xAngle, float yAngle, float zAngle);
}

namespace Projection {
   //TODO check if move function into Camera
   SquareMatrix view2ClipProjectiveMatrix(const float& right, const float& left, const float& near, const float& far, const float& top,
                                          const float& bottom);

   SquareMatrix onAxisView2ClipProjectiveMatrix(const float& right, const float& top, const float& near, const float& far);
   SquareMatrix onAxisFOV2ClipProjectiveMatrix(const Camera& camera);

   SquareMatrix orthogonalProjection(const float& right, const float& left, const float& near, const float& far, const float& top,
                                     const float& bottom);

   SquareMatrix onAxisOrthogonalProjection(const float& right, const float& top, const float& near, const float& far);


}

static float degree2Radiants(float degree) {
   float inv = 1.0f/180.0f;

   return degree * inv * M_PI;
}

static float radiants2Degree(float radiants) {
   return radiants * 180.0f * M_1_PI;
}


#endif //STANDARDMATRIX_H
