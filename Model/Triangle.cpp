#include "Triangle.h"

#include <memory>

Triangle::Triangle(Float3 &p0, Float3 &p1, Float3 &p2) {
   //TODO check move (if necessary or duplicate needed)
   //TODO optimise point to vector edit
   points = std::move(std::make_unique<Float3[]>(3));
   points.get()[0] = std::move(p0);
   points.get()[1] = std::move(p1);
   points.get()[2] = std::move(p2);

   normal = std::move((points.get()[1] - points.get()[0]).crossProduct(points.get()[2] - points.get()[0]));
}

const Float3& Triangle::getPoint0() const {
   return points.get()[0];
}

const Float3& Triangle::getPoint1() const {
   return points.get()[1];
}

const Float3& Triangle::getPoint2() const {
   return points.get()[2];
}

Float3 Triangle::getPoint(float u, float v) const {
   return Float3((1-u-v)*points[0] + u*points[1] + v*points[2]);
}

const Float3 &Triangle::getNormal() const {
   return normal;
}
