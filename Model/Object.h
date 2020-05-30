#ifndef BJECT_H
#define BJECT_H

#include <vector>
#include "Triangle.h"
#include "../Utilities/Color.h"
#include "../Vector/Float2.h"

class Object {
   std::vector<Triangle> vertices;
public:
   const vector<Triangle> &getVertices() const;

private:
   std::vector<Float2> textureUnwrap;

   float xTranslation;
   float yTranslation;
   float zTranslation;

   float xRotation;
   float yRotation;
   float zRotation;

   float xScale;
   float yScale;
   float zScale;

public:
   // TODO rule of 5
   Object(unsigned int preallocSize);

   //TODO check reference here
   //float* getWorldCoordinates();

   void addTriangle(Triangle triangle);

   const float& getXTranslation() const;

   void setXTranslation(const float& xTranslation);

   const float& getYTranslation() const;

   void setYTranslation(const float& yTranslation);

   const float& getZTranslation() const;

   void setZTranslation(const float& zTranslation);

   const float& getXRotation() const;

   void setXRotation(const float& xRotation);

   const float& getYRotation() const;

   void setYRotation(const float& yRotation);

   const float& getZRotation() const;

   void setZRotation(const float& zRotation);

   const float& getXScale() const;

   void setXScale(const float& xScale);

   const float& getYScale() const;

   void setYScale(const float& yScale);

   const float& getZScale() const;

   void setZScale(const float& zScale);
};

#endif //OBJECT_H
