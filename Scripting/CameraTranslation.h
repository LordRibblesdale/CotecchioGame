#ifndef COTECCHIOGAME_CAMERATRANSLATION_H
#define COTECCHIOGAME_CAMERATRANSLATION_H

#include "../Vector/Float3.h"

class CameraTranslation {
   Float3 initialPosition;
   Float3 finalPosition;

public:
   CameraTranslation(const Float3& initialPosition, const Float3& finalPosition);

   Float3 translateCamera(double time, double duration);
};


#endif //COTECCHIOGAME_CAMERATRANSLATION_H
