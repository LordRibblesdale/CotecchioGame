#include <fstream>
#include "Graphics/GL_Loader.h"
#include "rapidxml.hpp"

#if (_MSC_VER >= 1500)
#include "Graphics/assimp/include/assimp/Importer.hpp"
#include "Graphics/assimp/include/assimp/scene.h"
#include "Graphics/assimp/include/assimp/postprocess.h"
#else
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

// Argument as XML file importer
int main(int argc, char** argv) {
   std::string s("Test.fbx");
   Assimp::Importer importer;
   const aiScene* scene(importer.ReadFile(s, aiProcess_Triangulate));

   if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      return initialise();
   } else {
      std::cout << !scene << std::endl;
      std::cout << (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) << std::endl;
      std::cout << !scene->mRootNode << std::endl;

      return EXIT_FAILURE;
   }
}