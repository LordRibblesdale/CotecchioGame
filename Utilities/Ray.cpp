#include <limits>
#include "Ray.h"

Ray::Ray(Float3 origin, Float3 direction) : origin_(std::move(origin)), direction_(std::move(direction)) {}

bool Ray::isIntersecting(const Box &box) const {
   float tNear = std::numeric_limits<float>::min();
   float tFar = std::numeric_limits<float>::max();

   for (int i = 0; i < 3; ++i) {
      float inverseDirection = 1/ direction_.getVector().get()[i];
      //TODO     -V- check this minus sign (tFar < 0?)
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

TriangleIntersection& Ray::getTriangleIntersection(const Triangle &triangle) const {
   std::unique_ptr<TriangleIntersection> intersection(new TriangleIntersection(false, nullptr, nullptr));

   //TODO optimise memory?
   Float3 q(direction_.crossProduct(triangle.getPoints()[2] - triangle.getPoints()[0]));
   float determinant = q.dotProduct(triangle.getPoints()[1]);

   if (determinant != 0 || (determinant > -EPSILON && determinant < EPSILON)) {
      Float3 s(origin_ - triangle.getPoints()[2]);
      float a = 1 / determinant;

      float u = a*(q.dotProduct(s));

      if (u >= 0) {
         Float3 r(s.crossProduct(triangle.getPoints()[1] - triangle.getPoints()[0]));
         float v = a*(r.dotProduct(direction_));

         if (v >= 0 && (u+v) <= 1) {
            float t = a*(r.dotProduct(triangle.getPoints()[2] - triangle.getPoints()[0]));

            intersection = std::move(std::make_unique<TriangleIntersection>(true, new Float3(origin_ + t*direction_), new Float3(t, u, v)));
         }
       }
   }

   return *intersection.release();
}

Ray Ray::getReflectionOn(const Triangle &triangle, const TriangleIntersection &intersection) {
   return Ray(intersection.getIntersectedPoint(), direction_ - 2 * direction_.dotProduct(intersection.getBarycentricCoordinates()) * intersection.getBarycentricCoordinates());
}

Ray Ray::getRefractionOn(const Triangle &triangle, const TriangleIntersection &intersection) {
   //TODO implement Schlick equations
   return Ray(Float3(), Float3());
}

const Float3 &Ray::getOrigin() const {
   return origin_;
}

const Float3 &Ray::getDirection() const {
   return direction_;
}
