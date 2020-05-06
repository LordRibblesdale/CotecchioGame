#include <cmath>
#include "Float3.h"

Float3::Float3() : FloatVector(3, {0, 0, 0}) {}

Float3::Float3(const float &x, const float &y, const float &z) : FloatVector(3, {x, y, z}) {}

Float3::Float3(const Float3 &point) : FloatVector(3, {point.getX(), point.getY(), point.getZ()}) {}

Float3::Float3(Float3 &&point) : FloatVector(3, {point.getX(), point.getY(), point.getZ()}) {
   point.setX(0);
   point.setY(0);
   point.setZ(0);
}

Float3::~Float3() {}

Float3 &Float3::operator=(const Float3 &point) {
   setX(point.getX());
   setY(point.getY());
   setZ(point.getZ());

   return *this;
}

Float3 &Float3::operator=(Float3 &&point) {
   setX(point.getX());
   setY(point.getY());
   setZ(point.getZ());

   point.setX(0);
   point.setY(0);
   point.setZ(0);

   return *this;
}

Float3 Float3::operator+(const Float3 &point) {
   return Float3(getX() + point.getX(), getY() + point.getY(), getZ() + point.getZ());
}

Float3 operator+(const Float3 &point, const Float3 &point2) {
   return Float3(point.getX() + point2.getX(), point.getY() + point2.getY(), point.getZ() + point.getZ());
}

Float3& Float3::operator+=(const Float3 &point) {
   setX(getX() + point.getX());
   setY(getY() + point.getY());
   setZ(getZ() + point.getZ());

   return *this;
}

Float3 Float3::operator-(const Float3 &point) {
   return Float3(getX() - point.getX(), getY() - point.getY(), getZ() - point.getZ());
}

Float3 operator-(const Float3 &point, const Float3 &point2) {
   return Float3(point.getX() - point2.getX(), point.getY() - point2.getY(), point.getZ() - point.getZ());
}

Float3& Float3::operator-=(const Float3 &point) {
   setX(getX() - point.getX());
   setY(getY() - point.getY());
   setZ(getZ() - point.getZ());

   return *this;
}

Float3 Float3::operator*(const float &scalar) {
   return Float3(scalar * getX(), scalar * getY(), scalar * getZ());
}

Float3 operator*(const float& scalar, const Float3 &point) {
   return Float3(point.getX() * scalar, point.getY() * scalar, point.getZ() * scalar);
}

Float3& Float3::operator*=(const float &scalar) {
   setX(getX() * scalar);
   setY(getY() * scalar);
   setZ(getZ() * scalar);

   return *this;
}

float Float3::dotProduct(const Float3 &point) const {
   return getX() * point.getX() + getY() * point.getY() + getZ() * point.getZ();
}

Float3 Float3::crossProduct(const Float3& point) const {
   return Float3(getY() * point.getZ() - getZ() * point.getY(),
                 -(getX() * point.getZ() - getZ() * point.getX()),
                 getX() * point.getY() - getY() * point.getX());
}

float Float3::l2Norm() const {
   return sqrtf(getX() * getX() + getY() * getY() + getZ() * getZ());
}

void Float3::normalize() {
   float norm = l2Norm();
    if (norm != 0) {
       float n = 1/norm;
       setX(getX() * n);
       setY(getY() * n);
       setZ(getZ() * n);
    }
}

float Float3::getX() const {
   return get_vector().get()[0];
}

float Float3::getY() const {
   return get_vector().get()[1];
}

float Float3::getZ() const {
   return get_vector().get()[2];
}

void Float3::setX(const float &x) {
   get_vector().get()[0] = x;
}

void Float3::setY(const float &y) {
   get_vector().get()[1] = y;
}

void Float3::setZ(const float &z) {
   get_vector().get()[2] = z;
}
