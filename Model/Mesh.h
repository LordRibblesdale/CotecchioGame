#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "../Utilities/Color.h"
#include "../Vector/Float2.h"
#include "../Vector/Float3.h"
#include "Material.h"

class Mesh {
   std::vector<Float3> vertices;
   std::vector<Float3> normals;
   std::vector<Float2> textureUnwrap;
   std::vector<Color> vertexColors;
   std::vector<unsigned int> indices;

public:
   // TODO rule of 5
   Mesh(unsigned int preallocSize, bool hasTextures);
   ~Mesh();

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

   const Float3 &getDiffusive() const;

   void setDiffusive(const Float3 &diffusive);

   const Float3 &getSpecular() const;

   void setSpecular(const Float3 &specular);

   bool isPbr() const;

   void setPbr(bool pbr);
};

#endif //OBJECT_H
