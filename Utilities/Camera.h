#ifndef CAMERA_H
#define CAMERA_H

#include "../Vector/Float3.h"
#include "../Matrix/SquareMatrix.h"

class Camera {
   float near = 0;
   float far = 0;
   float left = 0;
   float right = 0;
   float bottom = 0;
   float top = 0;
   float angle = 0;
   float aspectRatio = 0;

   Float3 eye;
   Float3 lookAt;
   Float3 up;
public:
   float getAspectRatio() const;

   void setAspectRatio(float aspectRatio);

public:
   Camera(Float3 eye, Float3 lookAt);

   Camera(Float3 eye, Float3 lookAt, Float3 up);

   Camera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float angle, float aspectRatio);

   Camera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float bottom, float top, float left, float right, float angle);

   //TODO add Yaw and Pitch

   SquareMatrix world2ViewMatrix();

   void updateCamera();

   float getNear() const;

   void setNear(float near);

   float getFar() const;

   void setFar(float far);

   float getLeft() const;

   void setLeft(float left);

   float getRight() const;

   void setRight(float right);

   float getBottom() const;

   void setBottom(float bottom);

   float getTop() const;

   void setTop(float top);

   float getAngle() const;

   void setAngle(float angle);

   const Float3 &getEye() const;

   void setEye(const Float3 &eye);

   const Float3 &getLookAt() const;

   void setLookAt(const Float3 &lookAt);

   const Float3 &getUp() const;

   void setUp(const Float3 &up);
};

#endif //CAMERA_H
