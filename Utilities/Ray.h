#ifndef RAY_H
#define RAY_H

#include <memory>

#include "../Vector/Float3.h"
#include "../Model/Triangle.h"
#include "../Model/Box.h"

const float EPSILON = 0.00001f;

struct Intersection {
private:
   bool hasIntersected_;
   std::unique_ptr<Float3> intersectedPoint_ = nullptr;

public:
   Intersection(bool hasIntersected, Float3* intersectedPoint) : hasIntersected_(hasIntersected), intersectedPoint_(intersectedPoint) {}

   inline bool getHasIntersected() const {
      return hasIntersected_;
   }

   inline const Float3& getIntersectedPoint() const {
      return *intersectedPoint_;
   }
};

struct TriangleIntersection : public Intersection {
private:
   std::unique_ptr<Float3> barycentricCoordinates_;

public:
   TriangleIntersection(bool hasIntersected, Float3* intersectedPoint, Float3* barycentricCoordinates) : Intersection(hasIntersected, intersectedPoint) {
      barycentricCoordinates_ = std::move(std::unique_ptr<Float3>(barycentricCoordinates));
   }

   const Float3& getBarycentricCoordinates() const {
      return *barycentricCoordinates_;
   }
};

class Ray {
   Float3 origin_;
   Float3 direction_;

public:
   Ray(Float3 origin, Float3 direction);

   bool isIntersecting(const Box& box) const;

   TriangleIntersection getTriangleIntersection(const Triangle& triangle) const;

   Ray getReflectionOn(const Triangle& triangle, const TriangleIntersection& intersection);
   Ray getRefractionOn(const Triangle& triangle, const TriangleIntersection& intersection);

   const Float3 &getOrigin() const;
   const Float3 &getDirection() const;
};


#endif //RAY_H
