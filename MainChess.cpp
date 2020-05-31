#include "Graphics/GL_Loader_Ex1Chess.h"

// Argument as XML file importer
int main(int argc, char** argv) {
   int X_SUBDIVISION = 5;
   int Y_SUBDIVISION = 5;

   Object chess(2 * X_SUBDIVISION * Y_SUBDIVISION);

   // TODO create as shallow copy

   for (int i = 0; i < X_SUBDIVISION; ++i) {
      for (int j = 0; j < Y_SUBDIVISION; ++j) {
         Color c((j+i+1) % 2 == 0);
         Triangle first(Float3((-1.0f) + (2.0f/Y_SUBDIVISION)*j, 1.0f - (2.0f/X_SUBDIVISION)*i, 0),
                        Float3((-1.0f) + (2.0f/Y_SUBDIVISION)*j, 1.0f - (2.0f/X_SUBDIVISION)*(i+1), 0),
                        Float3((-1.0f) + (2.0f/Y_SUBDIVISION)*(j+1), 1.0f - (2.0f/X_SUBDIVISION)*i, 0),
                        c, c, c);

         Triangle second(Float3((-1.0f) + (2.0f/Y_SUBDIVISION)*j, 1.0f - (2.0f/X_SUBDIVISION)*(i+1), 0),
                         Float3((-1.0f) + (2.0f/Y_SUBDIVISION)*(j+1), 1.0f - (2.0f/X_SUBDIVISION)*(i+1), 0),
                         Float3((-1.0f) + (2.0f/Y_SUBDIVISION)*(j+1), 1.0f - (2.0f/X_SUBDIVISION)*i, 0),
                         c, c, c);

         /*
         for (int k = 0; k < 3; ++k) {
            std::cout << "Triangle first" << i+1 << " " << j+1 << ": " << first.getPoints()[k].toString() << std::endl;
            std::cout << "Triangle second" << i+1 << " " << j+1 << ": " << second.getPoints()[k].toString() << std::endl;
         }
          */

         chess.addTriangle(first);
         chess.addTriangle(second);
      }
   }

   vertices.emplace_back(chess);

   return initialise();
}