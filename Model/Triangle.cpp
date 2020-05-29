#include "Triangle.h"

#include <memory>

Triangle::Triangle(Float3 &p0, Float3 &p1, Float3 &p2) {
   //TODO check move (if necessary or duplicate needed)
   //TODO optimise point to vector edit
   points = std::move(std::make_unique<Float3[]>(3));
   normals = std::move(std::make_unique<Float3[]>(3));
   points[0] = std::move(p0);
   points[1] = std::move(p1);
   points[2] = std::move(p2);

   normals[0] = normals[1] = normals[2] = std::move((points.get()[1] - points.get()[0]).crossProduct(points.get()[2] - points.get()[0]));
}

const std::unique_ptr<Float3[]>& Triangle::getPoints() const {
   return points;
}

const std::unique_ptr<Float3[]> &Triangle::getNormals() const {
   return normals;
}


Float3 Triangle::getPoint(float u, float v) const {
   return Float3((1-u-v)*points[0] + u*points[1] + v*points[2]);
}

Float3 Triangle::getNormal(float u, float v) const {
   return Float3((1-u-v)*normals[0] + u*normals[1] + v*normals[2]);
}