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

   std::vector<Float3> tangents;
   std::vector<Float3> bitangents;

public:
   Mesh(unsigned int vertexPreallocSize, unsigned int indexPreallocSize, bool hasTextures);
   ~Mesh();

   void addVertex(const Float3& vertex);
   void addNormal(const Float3& normal);
   void addTextureUnwrap(const Float2 &uv);
   void addColor(const Color& color);
   void addIndex(unsigned int i);
   void addTangent(const Float3& tangent, unsigned int index1, unsigned int index2, unsigned int index3);
   void addBitangent(const Float3& bitangent, unsigned int index1, unsigned int index2, unsigned int index3);

   const std::vector<Float3> &getVertices() const;
   const std::vector<Float3> &getNormals() const;
   const std::vector<Float2> &getTextureUnwrap() const;
   const std::vector<Color> &getVertexColors() const;
   const std::vector<unsigned int> &getIndices() const;
   const std::vector<Float3>& getTangents() const;
   const std::vector<Float3>& getBitangents() const;

   const Float3 &getDiffusive() const;

   void setDiffusive(const Float3 &diffusive);

   const Float3 &getSpecular() const;

   void setSpecular(const Float3 &specular);

   bool isPbr() const;

   void setPbr(bool pbr);
};

#endif //OBJECT_H
