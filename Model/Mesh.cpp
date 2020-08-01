#include "Mesh.h"

Mesh::Mesh(unsigned int vertexPreallocSize, unsigned int indexPreallocSize, bool hasTextures) {
   vertices.reserve(vertexPreallocSize);
   normals.reserve(vertexPreallocSize);
   indices.reserve(indexPreallocSize);

   tangents.resize(vertexPreallocSize);
   bitangents.resize(vertexPreallocSize);

   hasTextures ? textureUnwrap.reserve(vertexPreallocSize) : vertexColors.reserve(vertexPreallocSize);
}

Mesh::~Mesh() {
   vertices.clear();
   normals.clear();
   indices.clear();
   textureUnwrap.clear();
   vertexColors.clear();

   tangents.clear();
   bitangents.clear();
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

void Mesh::addTangent(const Float3 &tangent, unsigned int index1, unsigned int index2, unsigned int index3) {
   tangents.at(index1) += tangent;
   tangents.at(index2) += tangent;
   tangents.at(index3) += tangent;
}

void Mesh::addBitangent(const Float3 &bitangent, unsigned int index1, unsigned int index2, unsigned int index3) {
   bitangents.at(index1) += bitangent;
   bitangents.at(index2) += bitangent;
   bitangents.at(index3) += bitangent;
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

const vector<Float3> &Mesh::getTangents() const {
   return tangents;
}

const vector<Float3> &Mesh::getBitangents() const {
   return bitangents;
}