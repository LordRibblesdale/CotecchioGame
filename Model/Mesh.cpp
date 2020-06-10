#include "Mesh.h"
#include "../Matrix/StandardMatrix.h"

Mesh::Mesh(unsigned int preallocSize)  {
   vertices.reserve(preallocSize);
   normals.reserve(preallocSize);
   textureUnwrap.reserve(preallocSize);
   vertexColors.reserve(preallocSize);
   indices.reserve(preallocSize);

   xTranslation = 0;
   yTranslation = 0;
   zTranslation = 0;

   xRotation = 0;
   yRotation = 0;
   zRotation = 0;

   xScale = 0;
   yScale = 0;
   zScale = 0;
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

SquareMatrix Mesh::getWorldCoordinates() const {
   // TODO complete
   // x y z r g b u v
   SquareMatrix translation(std::move(Transform::translateMatrix4(xTranslation, yTranslation, zTranslation)));
   //TODO check how to rotate with quaternions (check which axis to use)
   //SquareMatrix rotation(std::move(Rotation::rotationByQuaternion(Float4(1, 0, 0, 0), degree2Radiants(xRotation))));
   SquareMatrix xRotoMatrix(std::move(Rotation::rotationXAxisMatrix(xRotation)));
   SquareMatrix yRotoMatrix(std::move(Rotation::rotationXAxisMatrix(yRotation)));
   SquareMatrix zRotoMatrix(std::move(Rotation::rotationXAxisMatrix(zRotation)));
   SquareMatrix scale(Transform::scaleMatrix4(xScale, yScale, zScale));
   //TODO check apply here
   SquareMatrix transform(translation * zRotoMatrix * yRotoMatrix * xRotoMatrix * scale);

   return transform;
}

const float& Mesh::getXTranslation() const {
   return xTranslation;
}

void Mesh::setXTranslation(const float& xTranslation) {
   Mesh::xTranslation = xTranslation;
}

const float& Mesh::getYTranslation() const {
   return yTranslation;
}

void Mesh::setYTranslation(const float& yTranslation) {
   Mesh::yTranslation = yTranslation;
}

const float& Mesh::getZTranslation() const {
   return zTranslation;
}

void Mesh::setZTranslation(const float& zTranslation) {
   Mesh::zTranslation = zTranslation;
}

const float& Mesh::getXRotation() const {
   return xRotation;
}

void Mesh::setXRotation(const float& xRotation) {
   Mesh::xRotation = xRotation;
}

const float& Mesh::getYRotation() const {
   return yRotation;
}

void Mesh::setYRotation(const float& yRotation) {
   Mesh::yRotation = yRotation;
}

const float& Mesh::getZRotation() const {
   return zRotation;
}

void Mesh::setZRotation(const float& zRotation) {
   Mesh::zRotation = zRotation;
}

const float& Mesh::getXScale() const {
   return xScale;
}

void Mesh::setXScale(const float& xScale) {
   Mesh::xScale = xScale;
}

const float& Mesh::getYScale() const {
   return yScale;
}

void Mesh::setYScale(const float& yScale) {
   Mesh::yScale = yScale;
}

const float& Mesh::getZScale() const {
   return zScale;
}

void Mesh::setZScale(const float& zScale) {
   Mesh::zScale = zScale;
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
