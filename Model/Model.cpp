#include "Model.h"
#include "../Matrix/StandardMatrix.h"

Model::Model(std::string location, std::string name) : location(std::move(location)), name(std::move(name)) {}

void Model::processNode(aiNode *node, const aiScene *scene) {
   meshes.reserve(meshes.size() + node->mNumMeshes);

   for (int i = 0; i < node->mNumMeshes; ++i) {
      // Accesso alle mesh di interesse a partire dagli indici posseduti dal nodo
      processMesh(scene->mMeshes[node->mMeshes[i]]);
   }

   for (int i = 0; i < node->mNumChildren; ++i) {
      processNode(node->mChildren[i], scene);
   }
}

void Model::processMesh(aiMesh *mesh) {
   Mesh mesh1(mesh->mNumVertices, true);

   for (int i = 0; i < mesh->mNumVertices; ++i) {
      mesh1.addVertex(Float3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
      mesh1.addNormal(Float3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));

      mesh1.addTextureUnwrap(Float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
   }

   for (int i = 0; i < mesh->mNumFaces; ++i) {
      for (int j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
         mesh1.addIndex(mesh->mFaces[i].mIndices[j]);
      }
   }

   meshes.push_back(mesh1);
}

const vector<Mesh> &Model::getMeshes() const {
   return meshes;
}

SquareMatrix Model::getWorldCoordinates() const {
   float alpha1 = cosf(xRotation);
   float beta1 = sinf(xRotation);
   float alpha2 = cosf(yRotation);
   float beta2 = sinf(yRotation);
   float alpha3 = cosf(zRotation);
   float beta3 = sinf(zRotation);

   float a1b2 = alpha1 * beta2;
   float b1b2 = beta1 * beta2;

   return SquareMatrix(4, {xScale * alpha2 * alpha3, -yScale*(b1b2 * alpha3 + alpha1 * beta3), -zScale*(a1b2 * alpha3 - beta1 * beta3), xTranslation,
                           xScale * alpha2 * beta3, -yScale*(b1b2 * beta3 - alpha1 * alpha3), -zScale*(a1b2 * beta3 + beta1 * alpha3), yTranslation,
                           xScale * beta2, yScale * beta1 * alpha2, zScale * alpha1 * alpha2, zTranslation,
                           0, 0, 0, 1});
}

const float& Model::getXTranslation() const {
   return xTranslation;
}

void Model::setXTranslation(const float& xTranslation) {
   Model::xTranslation = xTranslation;
}

const float& Model::getYTranslation() const {
   return yTranslation;
}

void Model::setYTranslation(const float& yTranslation) {
   Model::yTranslation = yTranslation;
}

const float& Model::getZTranslation() const {
   return zTranslation;
}

void Model::setZTranslation(const float& zTranslation) {
   Model::zTranslation = zTranslation;
}

const float& Model::getXRotation() const {
   return xRotation;
}

void Model::setXRotation(const float& xRotation) {
   Model::xRotation = xRotation;
}

const float& Model::getYRotation() const {
   return yRotation;
}

void Model::setYRotation(const float& yRotation) {
   Model::yRotation = yRotation;
}

const float& Model::getZRotation() const {
   return zRotation;
}

void Model::setZRotation(const float& zRotation) {
   Model::zRotation = zRotation;
}

const float& Model::getXScale() const {
   return xScale;
}

void Model::setXScale(const float& xScale) {
   Model::xScale = xScale;
}

const float& Model::getYScale() const {
   return yScale;
}

void Model::setYScale(const float& yScale) {
   Model::yScale = yScale;
}

const float& Model::getZScale() const {
   return zScale;
}

void Model::setZScale(const float& zScale) {
   Model::zScale = zScale;
}

const string &Model::getName() const {
   return name;
}

const string &Model::getLocation() const {
   return location;
}
