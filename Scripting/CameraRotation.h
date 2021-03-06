#ifndef COTECCHIOGAME_CAMERAROTATION_H
#define COTECCHIOGAME_CAMERAROTATION_H

#include "../Matrix/SquareMatrix.h"
#include "../Vector/Float3.h"

class CameraRotation {
   //TODO implement camera rotation on different axis using finalPosition and initialPosition inclination
   SquareMatrix rotation;
   Float3 center;
   float angle;

public:
   CameraRotation(const Float3& initialPosition, const Float3& finalPosition, float radius);

   Float3 rotateCamera(const Float3 &position, double duration);
};


#endif //COTECCHIOGAME_CAMERAROTATION_H
