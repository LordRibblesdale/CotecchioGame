#include "Triangle.h"

#include <memory>

Triangle::Triangle(const Float3 &p0, const Float3 &p1, const Float3 &p2) {
   //TODO check move (if necessary or duplicate needed)
   //TODO optimise point to vector edit
   points = std::move(std::make_unique<Float3[]>(3));
   colors = std::move(std::make_unique<Color[]>(3));
   normals = std::move(std::make_unique<Float3[]>(3));
   points[0] = p0;
   points[1] = p1;
   points[2] = p2;

   normals[0] = normals[1] = normals[2] = std::move((points.get()[1] - points.get()[0]).crossProduct(points.get()[2] - points.get()[0]));
}

Triangle::Triangle(const Float3 &p0, const Float3 &p1, const Float3 &p2, const Color &c0, const Color &c1, const Color &c2) {
   //TODO check move (if necessary or duplicate needed)
   //TODO optimise point to vector edit
   points = std::move(std::make_unique<Float3[]>(3));
   colors = std::move(std::make_unique<Color[]>(3));
   normals = std::move(std::make_unique<Float3[]>(3));
   points[0] = p0;
   points[1] = p1;
   points[2] = p2;

   normals[0] = normals[1] = normals[2] = std::move((points.get()[1] - points.get()[0]).crossProduct(points.get()[2] - points.get()[0]));

   colors[0] = c0;
   colors[1] = c1;
   colors[2] = c2;
}

// TODO Check memory usage
Triangle::Triangle(const Triangle &triangle) {
   points = std::move(std::make_unique<Float3[]>(3));
   colors = std::move(std::make_unique<Color[]>(3));
   normals = std::move(std::make_unique<Float3[]>(3));

   for (int i = 0; i < 3; ++i) {
      points.get()[i] = triangle.getPoints()[i];
      colors.get()[i] = triangle.getColors()[i];
      normals.get()[i] = triangle.getNormals()[i];
   }
}

Triangle::Triangle(Triangle &&triangle) {
   points = std::move(const_cast<std::unique_ptr<Float3[]>&>(triangle.getPoints()));
   colors = std::move(const_cast<std::unique_ptr<Color[]>&>(triangle.getColors()));
   normals = std::move(const_cast<std::unique_ptr<Float3[]>&>(triangle.getNormals()));
}

Triangle::~Triangle() {
   points.reset();
   colors.reset();
   normals.reset();
}

const std::unique_ptr<Float3[]>& Triangle::getPoints() const {
   return points;
}

const std::unique_ptr<Color[]> &Triangle::getColors() const {
   return colors;
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

Float3 Triangle::getColor(float u, float v) const {
   return Color((1-u-v)*colors[0] + u*colors[1] + v*colors[2]);
}
