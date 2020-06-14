#ifndef COTECCHIOGAME_CAMERAROTATION_H
#define COTECCHIOGAME_CAMERAROTATION_H

#include "../Vector/Float4.h"
#include "../Matrix/SquareMatrix.h"

class CameraRotation {
   //TODO implement camera rotation on different axis using finalPosition and initialPosition inclination
   SquareMatrix quaternion;
   float angle;

public:
   CameraRotation(const Float3& initialPosition, const Float3& finalPosition, double duration, float radius);

   Float4 rotateCamera(const Float3 &position, double duration);
};


#endif //COTECCHIOGAME_CAMERAROTATION_H
