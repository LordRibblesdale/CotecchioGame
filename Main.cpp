#include "Graphics/GL_Loader.h"
#include "Graphics/Shaders.h"
#include "rapidxml.hpp"

#if (_MSC_VER >= 1500)
#include "Graphics/assimp/include/assimp/Importer.hpp"
#include "Graphics/assimp/include/assimp/scene.h"
#include "Graphics/assimp/include/assimp/postprocess.h"
#else
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#endif

// Argument as XML file importer
int main(int argc, char** argv) {
   std::string s("Test.fbx");
   Assimp::Importer importer;
   const aiScene* scene(importer.ReadFile(s, aiProcess_Triangulate));

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "Error ASSIMP_SCENE_LOADING: scene not loaded." << std::endl;

      return EXIT_FAILURE;
   } else {
      Model table;
      table.processNode(scene->mRootNode, scene);

      objects.emplace_back(table);

      return initialise();
      //return EXIT_SUCCESS;
   }
}