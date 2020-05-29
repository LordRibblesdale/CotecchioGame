#ifndef BJECT_H
#define BJECT_H

#include <vector>
#include "Triangle.h"
#include "../Utilities/Color.h"
#include "../Vector/Float2.h"

class Object {
   std::vector<Triangle> vertices;
   std::vector<Color> colors;
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
   Object(const std::initializer_list<Triangle>& triangles, const std::initializer_list<Color>& colors_, const std::initializer_list<Float2>& unwrap);

   //TODO check reference here
   //float* getWorldCoordinates();

   const float& getXTranslation() const;

   void setXTranslation(float xTranslation);

   const float& getYTranslation() const;

   void setYTranslation(float yTranslation);

   const float& getZTranslation() const;

   void setZTranslation(float zTranslation);

   const float& getXRotation() const;

   void setXRotation(float xRotation);

   const float& getYRotation() const;

   void setYRotation(float yRotation);

   const float& getZRotation() const;

   void setZRotation(float zRotation);

   const float& getXScale() const;

   void setXScale(float xScale);

   const float& getYScale() const;

   void setYScale(float yScale);

   const float& getZScale() const;

   void setZScale(float zScale);
};

#endif //OBJECT_H
