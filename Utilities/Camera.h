#ifndef CAMERA_H
#define CAMERA_H

#include "../Vector/Float3.h"
#include "../Matrix/SquareMatrix.h"

class Camera {
   Float3 eye;
   Float3 lootAt;
   Float3 up;

public:
   Camera(Float3 eye, Float3 lookAt);

   SquareMatrix world2ViewMatrix();
};

#endif //CAMERA_H
