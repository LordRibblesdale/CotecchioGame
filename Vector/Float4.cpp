#include <cmath>
#include "Float4.h"

Float4::Float4() : FloatVector(4, {0, 0, 0, 0}) {}

Float4::Float4(const float &x, const float &y, const float &z, const float &w) : FloatVector(4, {x, y, z, w}) {}

Float4::Float4(const Float4 &point) : FloatVector(4, {point.getX(), point.getY(), point.getZ(), point.getW()}) {}

Float4::Float4(Float4 &&point) : FloatVector(4, {point.getX(), point.getY(), point.getZ(), point.getW()}) {
   point.setX(0);
   point.setY(0);
   point.setZ(0);
   point.setW(0);
}

Float4::Float4(const Float3& float3, bool isPoint) : FloatVector(4, {float3.getX(), float3.getY(), float3.getZ()}) {
   if (isPoint) {
      setW(1);
   }
}

Float4::~Float4() {}

Float4 &Float4::operator=(const Float4 &point) {
   setX(point.getX());
   setY(point.getY());
   setZ(point.getZ());
   setW(point.getW());

   return *this;
}

Float4 &Float4::operator=(Float4 &&point) {
   setX(point.getX());
   setY(point.getY());
   setZ(point.getZ());
   setW(point.getW());

   point.setX(0);
   point.setY(0);
   point.setZ(0);
   point.setW(0);

   return *this;
}

Float4 Float4::operator+(const Float4 &point) {
   return Float4(getX() + point.getX(), getY() + point.getY(), getZ() + point.getZ(), getW() + point.getW());
}

Float4& Float4::operator+=(const Float4 &point) {
   setX(getX() + point.getX());
   setY(getY() + point.getY());
   setZ(getZ() + point.getZ());
   setW(getW() + point.getW());

   return *this;
}

Float4 Float4::operator-(const Float4 &point) {
   return Float4(getX() - point.getX(), getY() - point.getY(), getZ() - point.getZ(), getW() + point.getW());
}

Float4& Float4::operator-=(const Float4 &point) {
   setX(getX() - point.getX());
   setY(getY() - point.getY());
   setZ(getZ() - point.getZ());
   setW(getW() + point.getW());

   return *this;
}

Float4 Float4::operator*(const float &scalar) {
   return Float4(scalar * getX(), scalar * getY(), scalar * getZ(), scalar * getW());
}

Float4& Float4::operator*=(const float& scalar) {
   setX(getX() * scalar);
   setY(getY() * scalar);
   setZ(getZ() * scalar);
   setW(getW() * scalar);

   return *this;
}

float Float4::dotProduct(const Float4 &point) const {
   return getX() * point.getX() + getY() * point.getY() + getZ() * point.getZ() + getW() * point.getW();
}

/*
Float4 Float4::crossProduct(const Float4& point) const {
   return Float4(y * point.getZ() - z * point.getY(),
                 x * point.getZ() - z * point.getX(),
                 x * point.getY() - y * point.getX());
}
 */

float Float4::l2Norm() const {
   return sqrtf(getX() * getX() + getY() * getY() + getZ() * getZ() + getW() * getW());
}

void Float4::normalize() {
   float norm = l2Norm();
   if (norm != 0) {
      float n = 1/norm;
      setX(getX() * n);
      setY(getY() * n);
      setZ(getZ() * n);
      setW(getW() * n);
   }
}

float Float4::getX() const {
   return get_vector().get()[0];
}

float Float4::getY() const {
   return get_vector().get()[1];
}

float Float4::getZ() const {
   return get_vector().get()[2];
}

float Float4::getW() const {
   return get_vector().get()[3];
}

void Float4::setX(const float &x) {
   get_vector().get()[0] = x;
}

void Float4::setY(const float &y) {
   get_vector().get()[1] = y;
}

void Float4::setZ(const float &z) {
   get_vector().get()[2] = z;
}

void Float4::setW(const float &w) {
   get_vector().get()[3] = w;
}

Float3 Float4::getProjectionQuotient() {
   if (vector_.get()[3] != 0) {
      float inv = 1 / getW();

      return Float3(getX() * inv, getY() * inv, getZ() * inv);
   } else {
      throw vector_.get()[3];
   }
}
