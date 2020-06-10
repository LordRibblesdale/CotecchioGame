#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "Triangle.h"
#include "../Utilities/Color.h"
#include "../Vector/Float2.h"
#include "../Matrix/SquareMatrix.h"

class Mesh {

private:
   std::vector<Float3> vertices;
   std::vector<Float3> normals;
   std::vector<Float2> textureUnwrap;
   std::vector<Color> vertexColors;
   std::vector<unsigned int> indices;

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
   Mesh(unsigned int preallocSize);

   //TODO check reference here
   SquareMatrix getWorldCoordinates() const;

   void addVertex(const Float3& vertex);
   void addNormal(const Float3& normal);
   void addTextureUnwrap(const Float2 &uv);
   void addColor(const Color& color);
   void addIndex(unsigned int i);

   const std::vector<Float3> &getVertices() const;
   const vector<Float3> &getNormals() const;
   const vector<Float2> &getTextureUnwrap() const;
   const vector<Color> &getVertexColors() const;
   const vector<unsigned int> &getIndices() const;

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
