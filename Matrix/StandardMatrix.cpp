#include "StandardMatrix.h"

Float3 Rotation::axisXRotateVertex3(const Float3 &vector, const float &angleX) {
   float cosAngle = cosf(angleX);
   float sinAngle = sinf(angleX);
   SquareMatrix rotation(3, {1, 0, 0, 0, cosAngle, -sinAngle, 0, sinAngle, cosAngle});
   FloatVector rotatedVertex(std::move(rotation.multiply_vector(vector)));

   return Float3(*static_cast<Float3*>(&rotatedVertex));
}

Float3 Rotation::axisYRotateVertex3(const Float3 &vector, const float &angleY) {
   float cosAngle = cosf(angleY);
   float sinAngle = sinf(angleY);
   SquareMatrix rotation(3, {cosAngle, 0, sinAngle, 0, 1, 0, -sinAngle, 0, cosAngle});
   FloatVector rotatedVertex(std::move(rotation.multiply_vector(vector)));

   return Float3(*static_cast<Float3*>(&rotatedVertex));
}

Float3 Rotation::axisZRotateVertex3(const Float3 &vector, const float &angleZ) {
   float cosAngle = cosf(angleZ);
   float sinAngle = sinf(angleZ);
   SquareMatrix rotation(3, {cosAngle, -sinAngle, 0, sinAngle, cosAngle, 0, 0, 0, 1});
   FloatVector rotatedVertex(std::move(rotation.multiply_vector(vector)));

   return Float3(*static_cast<Float3*>(&rotatedVertex));
}

Float4 Rotation::quaternionAxisRotateVertex4(const Float4 &vector, Float4 &direction, const float &angle) {
   Float4 newVector;

   //TODO optimise here (remainder function)
   if (direction.l2_norm() != 0 && remainder(angle, 2*M_PI) != 0) {
      float sin = sinf(angle*0.5);
      float q1 = direction.get_x()*sin;
      float q2 = direction.get_y()*sin;
      float q3 = direction.get_z()*sin;
      float q4 = direction.get_w()*cosf(angle*0.5);
      //TODO check division here
      float s = 2/(q1*q1 + q2*q2 + q3*q3 + q4*q4);

      SquareMatrix quaternionMatrix(4, {1-s*(q2*q2 + q3*q3),   s*(q1*q2 - q4*q3),   s*(q1*q3 + q4*q2),   0,
                                        s*(q1*q2 + q4*q3),   1-s*(q1*q1 + q3*q3), s*(q2*q3 - q4*q1),   0,
                                        s*(q1*q3 - q4*q2),   s*(q2*q3 + q4*q1),   1-s*(q1*q1 + q2*q2), 0,
                                        0,                   0,                   0,                   1});

      FloatVector fv(quaternionMatrix.multiply_vector(vector));
      newVector = std::move(*static_cast<Float4*>(&fv));
   }

   return newVector;
}

SquareMatrix Transform::scaleTransform(const SquareMatrix &matrix, float scaleX, float scaleY, float scaleZ) {
   std::unique_ptr<float> newData(new float[matrix.get_dimension()*matrix.get_dimension()]);

   newData.get()[0] *= scaleX;
   newData.get()[matrix.get_dimension()] *= scaleY;
   newData.get()[matrix.get_dimension()*2] *= scaleZ;

   return SquareMatrix(matrix.get_dimension(), newData.release());
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
