#include "Mesh.h"

Mesh::Mesh(unsigned int vertexPreallocSize, unsigned int indexPreallocSize, bool hasTextures) {
   vertices.reserve(vertexPreallocSize);
   normals.reserve(vertexPreallocSize);
   indices.reserve(indexPreallocSize);

   hasTextures ? textureUnwrap.reserve(vertexPreallocSize) : vertexColors.reserve(vertexPreallocSize);
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

//void Mesh::addTangent(const Float3 &tangent) {
//   tangents.emplace_back(tangent);
//}
//
//void Mesh::addBitangent(const Float3 &bitangent) {
//   bitangents.emplace_back(bitangent);
//}

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

//const vector<Float3> &Mesh::getTangents() const {
//   return tangents;
//}
//
//const vector<Float3> &Mesh::getBitangents() const {
//   return bitangents;
//}

const Float3 &Mesh::getTangent() const {
   return tangent;
}

void Mesh::setTangent(const Float3 &tangent) {
   Mesh::tangent = tangent;
}

const Float3 &Mesh::getBitangent() const {
   return bitangent;
}

void Mesh::setBitangent(const Float3 &bitangent) {
   Mesh::bitangent = bitangent;
}