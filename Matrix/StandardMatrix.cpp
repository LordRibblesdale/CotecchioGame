#include "StandardMatrix.h"

Float3 Rotation::axisXRotateVertex3(const Float3 &vector, const float &angleX) {
   float cosAngle = cosf(angleX);
   float sinAngle = sinf(angleX);
   SquareMatrix rotation(3, {1, 0, 0, 0, cosAngle, -sinAngle, 0, sinAngle, cosAngle});

   return Float3(std::move(rotation.multiplyVector(vector)));
}

Float3 Rotation::axisYRotateVertex3(const Float3 &vector, const float &angleY) {
   float cosAngle = cosf(angleY);
   float sinAngle = sinf(angleY);
   SquareMatrix rotation(3, {cosAngle, 0, sinAngle, 0, 1, 0, -sinAngle, 0, cosAngle});

   return Float3(std::move(rotation.multiplyVector(vector)));
}

Float3 Rotation::axisZRotateVertex3(const Float3 &vector, const float &angleZ) {
   float cosAngle = cosf(angleZ);
   float sinAngle = sinf(angleZ);
   SquareMatrix rotation(3, {cosAngle, -sinAngle, 0, sinAngle, cosAngle, 0, 0, 0, 1});

   return Float3(std::move(rotation.multiplyVector(vector)));
}

Float4 Rotation::quaternionAxisRotateVertex4(const Float4 &vector, Float4 &direction, const float &angle) {
   //TODO optimise here (remainder function)
   if (direction.l2Norm() != 0) {
      if (remainder(angle, 2 * M_PI) != 0) {
         return direction;
      }

      float sin = sinf(angle * 0.5f);
      float q1 = direction.getX() * sin;
      float q2 = direction.getY() * sin;
      float q3 = direction.getZ() * sin;
      float q4 = direction.getW() * cosf(angle * 0.5f);
      //TODO check division here
      float s = 2/(q1*q1 + q2*q2 + q3*q3 + q4*q4);

      SquareMatrix quaternionMatrix(4, {1-s*(q2*q2 + q3*q3),   s*(q1*q2 - q4*q3),   s*(q1*q3 + q4*q2),   0,
                                        s*(q1*q2 + q4*q3),   1-s*(q1*q1 + q3*q3), s*(q2*q3 - q4*q1),   0,
                                        s*(q1*q3 - q4*q2),   s*(q2*q3 + q4*q1),   1-s*(q1*q1 + q2*q2), 0,
                                        0,                   0,                   0,                   1});

      return Float4(std::move(quaternionMatrix.multiplyVector(vector)));
   }

   return direction;
}

SquareMatrix Rotation::rotationXAxisMatrix(const float &angleX) {
   float cosAngle = cosf(angleX);
   float sinAngle = sinf(angleX);

   return SquareMatrix(3, {1, 0, 0, 0, cosAngle, -sinAngle, 0, sinAngle, cosAngle});
}

SquareMatrix Rotation::rotationYAxisMatrix(const float &angleY) {
   float cosAngle = cosf(angleY);
   float sinAngle = sinf(angleY);

   return SquareMatrix(3, {cosAngle, 0, sinAngle, 0, 1, 0, -sinAngle, 0, cosAngle});
}

SquareMatrix Rotation::rotationZAxisMatrix(const float &angleZ) {
   float cosAngle = cosf(angleZ);
   float sinAngle = sinf(angleZ);
   return SquareMatrix(3, {cosAngle, -sinAngle, 0, sinAngle, cosAngle, 0, 0, 0, 1});
}

SquareMatrix Rotation::rotationByQuaternion(Float4 direction, const float &angle) {
   if (direction.l2Norm() != 0 && remainder(angle, 2 * M_PI) != 0) {
      float sin = sinf(angle*0.5);
      float q1 = direction.getX() * sin;
      float q2 = direction.getY() * sin;
      float q3 = direction.getZ() * sin;
      float q4 = direction.getW() * cosf(angle * 0.5);
      //TODO check division here
      float s = 2/(q1*q1 + q2*q2 + q3*q3 + q4*q4);

      return SquareMatrix(4, {1-s*(q2*q2 + q3*q3),   s*(q1*q2 - q4*q3),   s*(q1*q3 + q4*q2),   0,
                                        s*(q1*q2 + q4*q3),   1-s*(q1*q1 + q3*q3), s*(q2*q3 - q4*q1),   0,
                                        s*(q1*q3 - q4*q2),   s*(q2*q3 + q4*q1),   1-s*(q1*q1 + q2*q2), 0,
                                        0,                   0,                   0,                   1});

   }

   //TODO check here
   return SquareMatrix(4, {});
}

SquareMatrix Transform::scaleMatrix4(float scaleX, float scaleY, float scaleZ) {
   return SquareMatrix(4, {scaleX, 0, 0, 0, 0, scaleY, 0, 0, 0, 0, scaleZ, 0, 0, 0, 0, 1});
}

SquareMatrix Transform::scaleTransform3(const SquareMatrix &matrix, float scaleX, float scaleY, float scaleZ) {
   std::unique_ptr<float> newData(new float[matrix.getDimension() * matrix.getDimension()]);

   newData.get()[0] *= scaleX;
   newData.get()[matrix.getDimension()] *= scaleY;
   newData.get()[matrix.getDimension() * 2] *= scaleZ;

   return SquareMatrix(matrix.getDimension(), newData.release());
}

SquareMatrix Transform::translateMatrix4(float x, float y, float z) {
   return SquareMatrix(4, {0, 0, 0, x,
                           0, 0, 0, y,
                           0, 0, 0, z,
                           0, 0, 0, 1});
}

SquareMatrix Projection::view2ClipProjection(const float& right, const float& left, const float &near, const float &far,
                                             const float &top, const float &bottom) {
   float invRL = 1/(right-left);
   float invTB = 1/(top-bottom);
   float invFN = 1/(far-near);
   return SquareMatrix(4, {2*near*invRL, 0,            (right+left)*invRL, 0,
                           0,          2*near*invTB, (top+bottom)*invTB, 0,
                           0,          0,            -(far+near)*invFN,  2*far*near*invFN,
                           0,          0,            -1,                 0});
}

SquareMatrix Projection::onAxisView2ClipProjection(const float& right, const float& top, const float& near, const float& far) {
   float invFN = 1/(far-near);
   return SquareMatrix(4, {near/right, 0, 0, 0,
                           0, near/top, 0, 0,
                           0, 0, -(far+near)*invFN, 2*far*near*invFN,
                           0, 0, -1,                0});
}

SquareMatrix Projection::orthogonalProjection(const float &right, const float &left, const float &near, const float &far,
                                 const float &top, const float &bottom) {
   float invRL = 1/(right-left);
   float invTB = 1/(top-bottom);
   float invFN = 1/(far-near);
   return SquareMatrix(4, {2*invRL, 0, 0, -(right+left)*invRL,
                           0, 2*invTB, 0, -(top+bottom)*invTB,
                           0, 0, -2*invFN, -(far+near)*invFN,
                           0, 0, 0, 1});
}

SquareMatrix Projection::onAxisOrthogonalProjection(const float& right, const float& top, const float& near, const float& far) {
   float invFN = 1/(far-near);
   return SquareMatrix(4, {1/right, 0, 0, 0,
                           0, near/top, 0, 0,
                           0, 0, -(far+near)*invFN, 2*far*near*invFN,
                           0, 0, -1,                0});
}
