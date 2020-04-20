//
// Created by Green on 19/03/2020.
//

/* Creare una classe Float2 che rappresenti un vettore bidimensionale di numeri float.
 * Fare l'overload di tutti gli operatori ( vettore + vettore, vettore + scalare, vettore + scalare,...)
 * implementare anche prodotti scalari/vettoriali/normalizzazione...
 * Testare tutti gli operatori in un main a parte.
 */

#include <cmath>
#include "Float2.h"

Float2::Float2() : FloatVector(2, {0, 0}) {}

Float2::Float2(const float& x, const float& y) : FloatVector(2, {x, y}) {}

Float2::Float2(const Float2 &point) : FloatVector(2, {point.getX(), point.getY()}) {}

Float2::Float2(Float2&& point) : FloatVector(2, {point.getX(), point.getY()}) {
   point.setX(0);
   point.setY(0);
}

Float2::~Float2() {}

//TODO missing "+=" e "-=" operators

Float2& Float2::operator=(const Float2 &point) {
   setX(point.getX());
   setY(point.getY());

   return *this;
}

Float2& Float2::operator=(Float2&& point) {
   setX(point.getX());
   setY(point.getY());

   point.setX(0);
   point.setY(0);

   return *this;
}

Float2 Float2::operator+(const Float2& point) {
   return Float2(getX() + point.getX(), getY() + point.getY());
}

Float2 Float2::operator-(const Float2& point) {
   return Float2(getX() - point.getX(), getY() - point.getY());
}

Float2 &Float2::operator+=(const Float2 &point) {
   setX(getX() +  point.getX());
   setY(getY() + point.getY());

   return *this;
}

Float2 &Float2::operator-=(const Float2 &point) {
   setX(getX() - point.getX());
   setY(getY() - point.getY());

   return *this;
}

Float2 Float2::operator*(const float& scalar) {
   return Float2(scalar*getX(), scalar*getY());
}

Float2& Float2::operator*=(const float& scalar) {
   setX(getX() * scalar);
   setY(getY() * scalar);
   return *this;
}

float Float2::dotProduct(const Float2& point) const {
   return getX() * point.getX() + getY() * point.getY();
}

Float2 Float2::crossProduct() const {
   return Float2(getY(), -getX());
}

float Float2::l2Norm() const {
   return sqrtf(getX()*getX() + getY()*getY());
}

Float2 Float2::normalize() {
   float norm = l2Norm();

   if (norm != 0) {
      setX(getX() / norm);
      setY(getY() / norm);
   }

   return *this;
}

float Float2::getX() const {
   return getVector().get()[0];
}

float Float2::getY() const {
   return getVector().get()[1];
}

void Float2::setX(const float& x) {
   getVector().get()[0] = x;
}

void Float2::setY(const float& y) {
   getVector().get()[1] = y;
}

ostream& operator<<(ostream& stream, const Float2& point) {
   return stream << "Point (" << point.getX() << ", " << point.getY() <<")";
}


Float2 operator*(const float& scalar, const Float2& point) {
   return Float2(point.getX()*scalar, point.getY()*scalar);
}
