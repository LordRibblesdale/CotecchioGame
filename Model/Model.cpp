#include "Model.h"
#include "../Matrix/StandardMatrix.h"

Model::Model(std::string location, std::string name) : location(std::move(location)), name(std::move(name)), local2World(SquareMatrix(4, {})) {}

unsigned long calculateMeshesSize(aiNode* node) {
   unsigned long size = node->mNumMeshes;

   for (int i = 0; i < node->mNumChildren; ++i) {
      size += calculateMeshesSize(node->mChildren[i]);
   }

   return size;
}

void Model::processNode(aiNode *node, const aiScene *scene) {
   unsigned long meshLenght = calculateMeshesSize(node);

   meshes.reserve(meshLenght);

   for (int i = 0; i < node->mNumMeshes; ++i) {
      // Accesso alle mesh di interesse a partire dagli indici posseduti dal nodo
      processMesh(scene->mMeshes[node->mMeshes[i]]);
   }

   for (int i = 0; i < node->mNumChildren; ++i) {
      processNode(node->mChildren[i], scene);
   }
}

void Model::processMesh(aiMesh *mesh) {
   unsigned int indexSize = 0;

   for (int i = 0; i < mesh->mNumFaces; ++i) {
      indexSize += mesh->mFaces[i].mNumIndices;
   }

   Mesh mesh1(mesh->mNumVertices, indexSize, true);

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

   for (int i = 0; i < mesh->mNumFaces; ++i) {
      for (int j = 0; j < mesh->mFaces[i].mNumIndices; j += 3) {
         // Impostazione del Tangent Space
         Float2 deltaUV1(std::move(mesh1.getTextureUnwrap().at(mesh->mFaces[i].mIndices[j+2]) - mesh1.getTextureUnwrap().at(mesh->mFaces[i].mIndices[j+1])));
         Float2 deltaUV2(std::move(mesh1.getTextureUnwrap().at(mesh->mFaces[i].mIndices[j+1]) - mesh1.getTextureUnwrap().at(mesh->mFaces[i].mIndices[j])));
         Float3 e1(std::move(mesh1.getVertices().at(mesh->mFaces[i].mIndices[j+2]) - mesh1.getVertices().at(mesh->mFaces[i].mIndices[j+1])));
         Float3 e2(std::move(mesh1.getVertices().at(mesh->mFaces[i].mIndices[j+1]) - mesh1.getVertices().at(mesh->mFaces[i].mIndices[j])));

         float invDet = 1.0f / (deltaUV1.getX()*deltaUV2.getY() - deltaUV2.getX()*deltaUV1.getY());

         mesh1.addTangent(invDet * Float3(deltaUV2.getY()*e1.getX() - deltaUV1.getY()*e2.getX(),
               deltaUV2.getY()*e1.getY() - deltaUV1.getY()*e2.getY(),
               deltaUV2.getY()*e1.getZ() - deltaUV1.getY()*e2.getZ()),
                     mesh->mFaces[i].mIndices[j], mesh->mFaces[i].mIndices[j+1], mesh->mFaces[i].mIndices[j+2]);

         mesh1.addBitangent(invDet * Float3(-deltaUV2.getX()*e1.getX() + deltaUV1.getX()*e2.getX(),
               -deltaUV2.getX()*e1.getY() + deltaUV1.getX()*e2.getY(),
               -deltaUV2.getX()*e1.getZ() + deltaUV1.getX()*e2.getZ()),
                     mesh->mFaces[i].mIndices[j], mesh->mFaces[i].mIndices[j+1], mesh->mFaces[i].mIndices[j+2]);
      }
   }

   meshes.push_back(std::move(mesh1));
}

const vector<Mesh> &Model::getMeshes() const {
   return meshes;
}

const SquareMatrix& Model::getWorldCoordinates() {
   float alpha1 = cosf(xRotation);
   float beta1 = sinf(xRotation);
   float alpha2 = cosf(yRotation);
   float beta2 = sinf(yRotation);
   float alpha3 = cosf(zRotation);
   float beta3 = sinf(zRotation);

   float a1b2 = alpha1 * beta2;
   float b1b2 = beta1 * beta2;

   local2World = std::move(SquareMatrix(4, {xScale * alpha2 * alpha3, -yScale*(b1b2 * alpha3 + alpha1 * beta3), -zScale*(a1b2 * alpha3 - beta1 * beta3), xTranslation,
                                            xScale * alpha2 * beta3, -yScale*(b1b2 * beta3 - alpha1 * alpha3), -zScale*(a1b2 * beta3 + beta1 * alpha3), yTranslation,
                                            xScale * beta2, yScale * beta1 * alpha2, zScale * alpha1 * alpha2, zTranslation,
                                            0, 0, 0, 1}));

   return local2World;
}

float Model::getHighestZPoint() {
   float z = 0;

   for (const Mesh& mesh : meshes) {
      for (const Float3& vertex : mesh.getVertices()) {
         // Local to World matrix rewrite
         float tmp = sinf(xRotation) * yScale * vertex.getY() + cosf(xRotation) * zScale * vertex.getZ();
         float final = sinf(yRotation) * xScale * vertex.getX() + cosf(yRotation) * tmp + zTranslation;

         if (z < final) {
            z = final;
         }
      }
   }

   return z;
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

bool Model::doesHaveTextures() const {
   return hasTextures;
}

void Model::setTexturesEnabled(bool hasTextures) {
   Model::hasTextures = hasTextures;
}

bool Model::doesNeedNoCulling() const {
   return needsNoCulling;
}

void Model::setNeedsNoCulling(bool needsNoCulling) {
   Model::needsNoCulling = needsNoCulling;
}

const SquareMatrix &Model::getLocal2WorldMatrix() const {
   return local2World;
}
