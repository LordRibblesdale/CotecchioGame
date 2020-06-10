#include "Model.h"

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
