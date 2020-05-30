#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include "../Vector/Float3.h"
#include "../Utilities/Color.h"

class Triangle {
   //TODO check color add here or directly reading a XML file
   //TODO add normal reading via interpolation
   std::unique_ptr<Float3[]> points;
   std::unique_ptr<Color[]> colors;
   std::unique_ptr<Float3[]> normals;

public:
   // TODO add constructors (normals and colors)
   Triangle(const Float3& p0, const Float3& p1, const Float3& p2);
   Triangle(const Float3& p0, const Float3& p1, const Float3& p2, const Color& c0, const Color& c1, const Color& c2);
   Triangle(const Triangle& triangle);
   Triangle(Triangle&& triangle);
   ~Triangle();

   const std::unique_ptr<Float3[]>& getPoints() const;
   const std::unique_ptr<Color[]>& getColors() const;
   const std::unique_ptr<Float3[]>& getNormals() const;

   Float3 getPoint(float u, float v) const;
   Float3 getColor(float u, float v) const;
   Float3 getNormal(float u, float v) const;
};

#endif //TRIANGLE_H
