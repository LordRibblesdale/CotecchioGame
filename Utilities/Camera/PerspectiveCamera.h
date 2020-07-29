#ifndef PROJECTIVECAMERA_H
#define PROJECTIVECAMERA_H

#include "Camera.h"

class PerspectiveCamera : public Camera {
   float viewAngle = 0;
   float aspectRatio = 0;

public:
   PerspectiveCamera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float angle, float aspectRatio);
   PerspectiveCamera(Float3 eye, Float3 lookAt, Float3 up, float near, float zoom, float far, float angle, float aspectRatio);
   PerspectiveCamera(Float3 eye, Float3 lookAt, Float3 up, float near, float far, float bottom, float top, float left, float right, float angle);

   SquareMatrix view2ClipProjectiveMatrix();
   void view2ClipProjectiveMatrix(SquareMatrix& matrix);
   SquareMatrix onAxisView2ClipProjectiveMatrix();
   void onAxisView2ClipProjectiveMatrix(SquareMatrix& matrix);

   float getAngle() const;
   void setAngle(float angle);

   float getAspectRatio() const;
   void setAspectRatio(float aspectRatio);
};


#endif //PROJECTIVECAMERA_H
