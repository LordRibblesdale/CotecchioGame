#ifndef BOX_H
#define BOX_H

#include "../Vector/Float3.h"

struct Box {
private:
   Float3 initialPoint_;
   Float3 finalPoint_;

public:
   // TODO rule of 5
   Box();

   Box(const Float3& initialPoint, const Float3& finalPoint);

   

   const Float3 &getInitialPoint() const {
      return initialPoint_;
   }

   const Float3 &getFinalPoint() const {
      return finalPoint_;
   }
};


#endif //BOX_H
