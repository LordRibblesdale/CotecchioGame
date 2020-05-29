#ifndef CAMERA_H
#define CAMERA_H

#include "../Vector/Float3.h"
#include "../Matrix/SquareMatrix.h"

class Camera {
   Float3 eye;
   Float3 lootAt;
   Float3 up;

public:
   //TODO add constructor w/ up vector
   Camera(Float3 eye, Float3 lookAt);

   //TODO add Yaw and Pitch

   SquareMatrix world2ViewMatrix();
};

#endif //CAMERA_H
