#include "Mesh.h"

Mesh::Mesh(unsigned int preallocSize, bool hasTextures)  {
   vertices.reserve(preallocSize);
   normals.reserve(preallocSize);
   indices.reserve(preallocSize);

   hasTextures ? textureUnwrap.reserve(preallocSize) : vertexColors.reserve(preallocSize);
}

Mesh::~Mesh() {
   vertices.clear();
   normals.clear();
   indices.clear();
   textureUnwrap.clear();
   vertexColors.clear();
}

void Mesh::addVertex(const Float3& vertex) {
   vertices.emplace_back(vertex);
}

void Mesh::addNormal(const Float3& normal) {
   normals.emplace_back(normal);
}

void Mesh::addTextureUnwrap(const Float2& uv) {
   textureUnwrap.emplace_back(uv);
}

void Mesh::addColor(const Color &color) {
   vertexColors.emplace_back(color);
}

void Mesh::addIndex(unsigned int i) {
   indices.emplace_back(i);
}

const vector<Float3> &Mesh::getVertices() const {
   return vertices;
}

const vector<Float3> &Mesh::getNormals() const {
   return normals;
}

const vector<Float2> &Mesh::getTextureUnwrap() const {
   return textureUnwrap;
}

const vector<Color> &Mesh::getVertexColors() const {
   return vertexColors;
}

const vector<unsigned int> &Mesh::getIndices() const {
   return indices;
}
