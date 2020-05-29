#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include "../Vector/Float3.h"

class Triangle {
   //TODO check color add here or directly reading a XML file
   //TODO add normal reading via interpolation
   std::unique_ptr<Float3[]> points;
   std::unique_ptr<Float3[]> normals;

public:
   Triangle(Float3& p0, Float3& p1, Float3& p2);

   const std::unique_ptr<Float3[]>& getPoints() const;
   const std::unique_ptr<Float3[]>& getNormals() const;

   Float3 getPoint(float u, float v) const;
   Float3 getNormal(float u, float v) const;
};

#endif //TRIANGLE_H
