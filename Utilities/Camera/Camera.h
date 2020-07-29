#ifndef CAMERA_H
#define CAMERA_H

#include "../../Vector/Float3.h"
#include "../../Matrix/SquareMatrix.h"

class Camera {
protected:
   // TODO set if camera is on axis or not

   float near = 0;
   float far = 0;
   float left = 0;
   float right = 0;
   float bottom = 0;
   float top = 0;

   float sensibility = 0.1f;

   Float3 eye;
   Float3 lookAt;
   Float3 up;

   Camera(Float3 eye, Float3 lookAt);
   Camera(Float3 eye, Float3 lookAt, Float3 up);
   Camera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float bottom, float top, float left, float right);

public:
   SquareMatrix world2ViewMatrix();
   void world2ViewMatrix(SquareMatrix& matrix);
   SquareMatrix view2WorldMatrix();
   void view2WorldMatrix(SquareMatrix& matrix);

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

   const Float3 &getEye() const;
   void setEye(const Float3 &eye);

   const Float3 &getLookAt() const;

   const Float3 &getUp() const;
   void setUp(const Float3 &up);

   float getSensibility() const;
   void setSensibility(float sensibility);

   void setLookAt(const Float3 &lookAt);
};

#endif //CAMERA_H
