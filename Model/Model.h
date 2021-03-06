#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>
#include "../Model/Mesh.h"
#include "../Matrix/SquareMatrix.h"

// TODO check if possible to change model loading pipeline (assigning texture GLuint & Material here)

class Model {
   std::string location;
   std::string name;
   std::vector<Mesh> meshes;

   float xTranslation = 0;
   float yTranslation = 0;
   float zTranslation = 0;

   float xRotation = 0;
   float yRotation = 0;
   float zRotation = 0;

   float xScale = 1;
   float yScale = 1;
   float zScale = 1;

   bool hasTextures;
   bool needsNoCulling;

   SquareMatrix local2World;
public:
   Model(std::string location, std::string name);

   const vector<Mesh>& getMeshes() const;

   void processNode(aiNode* node, const aiScene* scene);
   void processMesh(aiMesh* mesh);

   const SquareMatrix& getWorldCoordinates();

   const SquareMatrix& getLocal2WorldMatrix() const;

   float getHighestZPoint();

   float getFurthestXPoint();

   const string &getName() const;
   const string &getLocation() const;

   bool doesHaveTextures() const;
   void setTexturesEnabled(bool hasTextures);

   bool doesNeedNoCulling() const;
   void setNeedsNoCulling(bool needsNoCulling);

   const float& getXTranslation() const;
   void setXTranslation(const float& xTranslation);

   const float& getYTranslation() const;
   void setYTranslation(const float& yTranslation);

   const float& getZTranslation() const;
   void setZTranslation(const float& zTranslation);

   const float& getXRotation() const;
   void setXRotation(const float& xRotation);

   const float& getYRotation() const;
   void setYRotation(const float& yRotation);

   const float& getZRotation() const;
   void setZRotation(const float& zRotation);

   const float& getXScale() const;
   void setXScale(const float& xScale);

   const float& getYScale() const;
   void setYScale(const float& yScale);

   const float& getZScale() const;
   void setZScale(const float& zScale);
};

#endif //MODEL_H
