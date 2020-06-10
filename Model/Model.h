#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>
#include "Material.h"
#include "Mesh.h"

class Model {
   std::vector<Mesh> meshes;
   // TODO add booleans for properties

public:
   const vector<Mesh> &getMeshes() const;

   void processNode(aiNode* node, const aiScene* scene);
   void processMesh(aiMesh* mesh, const aiScene* scene);
};

#endif //MODEL_H
