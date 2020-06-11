#include "Model.h"
#include "../Matrix/StandardMatrix.h"

void Model::processNode(aiNode *node, const aiScene *scene) {
   for (int i = 0; i < node->mNumMeshes; ++i) {
      // Accesso alle mesh di interesse a partire dagli indici posseduti dal nodo
      processMesh(scene->mMeshes[node->mMeshes[i]], scene);
   }
   for (int i = 0; i < node->mNumChildren; ++i) {
      processNode(node->mChildren[i], scene);
   }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene) {
   Mesh mesh1(mesh->mNumVertices);
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

   //TODO add material
}

const vector<Mesh> &Model::getMeshes() const {
   return meshes;
}

SquareMatrix Model::getWorldCoordinates() const {
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
