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

   //
   // std::vector<Float3> tangents;
   //std::vector<Float3> bitangents;

   Float3 tangent;
   Float3 bitangent;

public:
   // TODO rule of 5
   Mesh(unsigned int vertexPreallocSize, unsigned int indexPreallocSize, bool hasTextures);
   ~Mesh();

   void addVertex(const Float3& vertex);
   void addNormal(const Float3& normal);
   void addTextureUnwrap(const Float2 &uv);
   void addColor(const Color& color);
   void addIndex(unsigned int i);

   //void addTangent(const Float3& tangent);
   //void addBitangent(const Float3& bitangent);

   const std::vector<Float3> &getVertices() const;
   const vector<Float3> &getNormals() const;
   const vector<Float2> &getTextureUnwrap() const;
   const vector<Color> &getVertexColors() const;
   const vector<unsigned int> &getIndices() const;

   const vector<Float3> &getTangents() const;

   const vector<Float3> &getBitangents() const;

   const Float3 &getDiffusive() const;

   void setDiffusive(const Float3 &diffusive);

   const Float3 &getSpecular() const;

   void setSpecular(const Float3 &specular);

   bool isPbr() const;

   void setPbr(bool pbr);


    const Float3 &getTangent() const;

    void setTangent(const Float3 &tangent);

    const Float3 &getBitangent() const;

    void setBitangent(const Float3 &bitangent);
};

#endif //OBJECT_H
