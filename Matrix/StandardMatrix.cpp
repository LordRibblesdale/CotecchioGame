#include <iostream>
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

Float4 Rotation::quaternionAxisRotateVertex4(const Float4&vector, const Float3& direction, const float &angle) {
   if (direction.l2Norm() != 0 && remainder(angle, 2 * M_PI) != 0) {
      return Float4(std::move(rotationByQuaternion(direction, angle).multiplyVector(vector)));
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

SquareMatrix Rotation::rotationXAxisMatrix4(const float &angleX) {
   float cosAngle = cosf(angleX);
   float sinAngle = sinf(angleX);

   return SquareMatrix(4, {1, 0, 0, 0, 0, cosAngle, -sinAngle, 0, 0, sinAngle, cosAngle, 0, 0, 0, 0, 1});
}

SquareMatrix Rotation::rotationYAxisMatrix4(const float &angleY) {
   float cosAngle = cosf(angleY);
   float sinAngle = sinf(angleY);

   return SquareMatrix(4, {cosAngle, 0, sinAngle, 0, 0, 1, 0, 0, -sinAngle, 0, cosAngle, 0, 0, 0, 0, 1});
}

SquareMatrix Rotation::rotationZAxisMatrix4(const float &angleZ) {
   float cosAngle = cosf(angleZ);
   float sinAngle = sinf(angleZ);
   return SquareMatrix(4, {cosAngle, -sinAngle, 0, 0, sinAngle, cosAngle, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1});
}


SquareMatrix Rotation::rotationByQuaternion(const Float3& direction, const float &angle) {
   if (direction.l2Norm() != 0 && remainder(angle, 2 * M_PI) != 0) {
      float sin = sinf(angle * 0.5f);
      float q1 = direction.getX() * sin;
      float q2 = direction.getY() * sin;
      float q3 = direction.getZ() * sin;
      float q4 = cosf(angle * 0.5f);
      float s = 2/(q1*q1 + q2*q2 + q3*q3 + q4*q4);

      return SquareMatrix(4, {1-s*(q2*q2 + q3*q3),   s*(q1*q2 - q4*q3),   s*(q1*q3 + q4*q2),   0,
                              s*(q1*q2 + q4*q3),   1-s*(q1*q1 + q3*q3), s*(q2*q3 - q4*q1),   0,
                              s*(q1*q3 - q4*q2),   s*(q2*q3 + q4*q1),   1-s*(q1*q1 + q2*q2), 0,
                              0,                   0,                   0,                   1});
   }

   return Transform::scaleMatrix4(1, 1, 1);
}

SquareMatrix Transform::scaleMatrix4(float scaleX, float scaleY, float scaleZ) {
   return SquareMatrix(4, {scaleX, 0, 0, 0, 0, scaleY, 0, 0, 0, 0, scaleZ, 0, 0, 0, 0, 1});
}

SquareMatrix Transform::scaleTransform3(const SquareMatrix &matrix, float scaleX, float scaleY, float scaleZ) {
   FloatArray newData(matrix.getDimension(), {});

   unsigned int dim = matrix.getDimension();
   short j = 1;

   for (int i = 0; i < dim * dim; ++i) {
      if (i == j * dim) {
         ++j;
      }

      if (j == 1) {
         matrix.getArray()[i] = matrix.getArray()[i] * scaleX;
      } else if (j == 2) {
         matrix.getArray()[i] = matrix.getArray()[i] * scaleY;
      } else if (j == 3) {
         matrix.getArray()[i] = matrix.getArray()[i] * scaleZ;
      }
   }

   return SquareMatrix(std::move(newData));
}

SquareMatrix Transform::translateMatrix4(float x, float y, float z) {
   return SquareMatrix(4, {1, 0, 0, x,
                           0, 1, 0, y,
                           0, 0, 1, z,
                           0, 0, 0, 1});
}

Float3 Transform::scaleVector3(const Float3& vector, float scaleX, float scaleY, float scaleZ) {
   return Float3(vector.getX()*scaleX, vector.getY()*scaleY, vector.getZ()*scaleZ);
}