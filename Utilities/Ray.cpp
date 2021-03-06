#include <limits>
#include <iostream>
#include "Ray.h"

Ray::Ray(Float3 origin, Float3 direction) : origin_(std::move(origin)), direction_(std::move(direction)) {}

bool Ray::isIntersecting(const Box &box) const {
   float tNear = std::numeric_limits<float>::min();
   float tFar = std::numeric_limits<float>::max();

   for (int i = 0; i < 3; ++i) {
      float inverseDirection = 1/ direction_.getVector().get()[i];
      float t1 = -(box.getInitialPoint().getVector().get()[i] - origin_.getVector().get()[i]) * inverseDirection;
      float t2 = -(box.getFinalPoint().getVector().get()[i] - origin_.getVector().get()[i]) * inverseDirection;

      if (t1 > t2) {
         std::swap(t1, t2);
      }

      if (t1 > tNear) {
         tNear = t1;
      }

      if (t2 < tFar) {
         tFar = t2;
      }

      if (tNear > tFar) {
         return false;
      }

      if (tFar < 0) {
         return false;
      }
   }

   return true;
}

TriangleIntersection Ray::getTriangleIntersection(const Triangle &triangle) const {
   TriangleIntersection intersection(false, nullptr, nullptr);

   Float3 e2(std::move(triangle.getPoints()[2] - triangle.getPoints()[0]));
   Float3 e1(std::move(triangle.getPoints()[1] - triangle.getPoints()[0]));
   Float3 q(direction_.crossProduct(e2));
   float determinant = q.dotProduct(e1);

   if (determinant < -EPSILON || determinant > EPSILON) {
      Float3 s(origin_ - triangle.getPoints()[0]);
      float a = 1 / determinant;
      float u = a*(q.dotProduct(s));

      if (u >= 0 && u <= 1) {
         Float3 r(std::move(s.crossProduct(e1)));
         float v = a*(r.dotProduct(direction_));

         if (v >= 0 && (u+v) <= 1) {
            float t = a*(r.dotProduct(e2));

            if (t > EPSILON) {
               intersection = std::move(TriangleIntersection(true, new Float3(origin_ + t*direction_), new Float3(t, u, v)));
            }
         }
       }
   }

   return intersection;
}

const Float3 &Ray::getOrigin() const {
   return origin_;
}

const Float3 &Ray::getDirection() const {
   return direction_;
}
