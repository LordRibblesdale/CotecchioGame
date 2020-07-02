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
   float viewAngle = 0;
   float aspectRatio = 0;

   float yawAngle = 0;

   float sensibility = 0.1f;

   Float3 eye;
   Float3 lookAt;
   Float3 up;

public:
   Camera(Float3 eye, Float3 lookAt);

   Camera(Float3 eye, Float3 lookAt, Float3 up);

   Camera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float angle, float aspectRatio);

   Camera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float bottom, float top, float left, float right, float angle);

   SquareMatrix world2ViewMatrix();
   SquareMatrix view2WorldMatrix();

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

   float getYawAngle() const;

   void setYawAngle(float yawAngle);

   float getAspectRatio() const;

   void setAspectRatio(float aspectRatio);

   float getSensibility() const;

   void setSensibility(float sensibility);
};

#endif //CAMERA_H
