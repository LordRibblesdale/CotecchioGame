#include "Triangle.h"

#include <memory>

Triangle::Triangle(const Float3 &p0, const Float3 &p1, const Float3 &p2) {
   //TODO optimise point to vector edit
   points = std::move(std::make_unique<Float3[]>(3));
   points[0] = p0;
   points[1] = p1;
   points[2] = p2;
}

Triangle::Triangle(const Float4 &p0, const Float4 &p1, const Float4 &p2) {
   //TODO optimise point to vector edit
   points = std::move(std::make_unique<Float3[]>(3));
   points[0] = std::move(p0.getFloat3());
   points[1] = std::move(p1.getFloat3());
   points[2] = std::move(p2.getFloat3());
}

Triangle::Triangle(const Triangle &triangle) {
   points = std::move(std::make_unique<Float3[]>(3));

   for (int i = 0; i < 3; ++i) {
      points.get()[i] = triangle.getPoints()[i];
   }
}

Triangle::Triangle(Triangle &&triangle) {
   points = std::move(const_cast<std::unique_ptr<Float3[]>&>(triangle.getPoints()));
}

Triangle::~Triangle() {
   points.reset();
}

const std::unique_ptr<Float3[]>& Triangle::getPoints() const {
   return points;
}

Float3 Triangle::getPoint(float u, float v) const {
   return Float3((1-u-v)*points[0] + u*points[1] + v*points[2]);
}
