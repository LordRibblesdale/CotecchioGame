#ifndef STANDARDMATRIX_H
#define STANDARDMATRIX_H

#include "../Vector/Float4.h"
#include "SquareMatrix.h"
#include "../Utilities/Camera.h"
#include <cmath>

#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_1_PI		0.31830988618379067154

//TODO add Identity

namespace Rotation {
   SquareMatrix rotationXAxisMatrix(const float& angleX);
   SquareMatrix rotationYAxisMatrix(const float& angleY);
   SquareMatrix rotationZAxisMatrix(const float& angleZ);
   SquareMatrix rotationXAxisMatrix4(const float& angleX);
   SquareMatrix rotationYAxisMatrix4(const float& angleY);
   SquareMatrix rotationZAxisMatrix4(const float& angleZ);

   SquareMatrix rotationByQuaternion(const Float4& direction, const float& angle);

   Float3 axisXRotateVertex3(const Float3 &vector, const float& angleX);
   Float3 axisYRotateVertex3(const Float3 &vector, const float& angleY);
   Float3 axisZRotateVertex3(const Float3 &vector, const float& angleZ);

   Float4 quaternionAxisRotateVertex4(const Float4 &vector, const Float4 &direction, const float &angle);
}

namespace Transform {
   // TODO optimise variable gathering
   SquareMatrix scaleMatrix4(float scaleX, float scaleY, float scaleZ);
   SquareMatrix scaleTransform3(const SquareMatrix &matrix, float scaleX, float scaleY, float scaleZ);

   SquareMatrix translateMatrix4(float x, float y, float z);

   Float3 scaleVector3(const Float3& vector, float scaleX, float scaleY, float scaleZ);
}

namespace Projection {
   SquareMatrix view2ClipProjectiveMatrix(const float& right, const float& left, const float& near, const float& far, const float& top,
                                          const float& bottom);

   SquareMatrix onAxisView2ClipProjectiveMatrix(const float& right, const float& top, const float& near, const float& far);
   SquareMatrix onAxisFOV2ClipProjectiveMatrix(const Camera& camera);

   SquareMatrix orthogonalProjection(const float& right, const float& left, const float& near, const float& far, const float& top,
                                     const float& bottom);

   SquareMatrix onAxisOrthogonalProjection(const float& right, const float& top, const float& near, const float& far);
   SquareMatrix onAxisOrthogonalProjectionByCamera(const Camera& camera);
}

static float degree2Radiants(float degree) {
   float inv = 1.0f/180.0f;

   return degree * inv * M_PI;
}

static float radiants2Degree(float radiants) {
   return radiants * 180.0f * M_1_PI;
}


#endif //STANDARDMATRIX_H
