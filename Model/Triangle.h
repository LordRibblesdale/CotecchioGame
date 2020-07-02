#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include "../Vector/Float3.h"
#include "../Utilities/Color.h"
#include "../Vector/Float4.h"

class Triangle {
   //TODO check color add here or directly reading a XML file
   //TODO add normal reading via interpolation
   std::unique_ptr<Float3[]> points;


public:
   // TODO add constructors (normals and colors)
   Triangle(const Float3& p0, const Float3& p1, const Float3& p2);
   Triangle(const Float4& p0, const Float4& p1, const Float4& p2);
   Triangle(const Triangle& triangle);
   Triangle(Triangle&& triangle);
   ~Triangle();

   const std::unique_ptr<Float3[]>& getPoints() const;

   Float3 getPoint(float u, float v) const;
};

#endif //TRIANGLE_H
