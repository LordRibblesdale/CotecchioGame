#ifndef COTECCHIOGAME_CARDMOVING_H
#define COTECCHIOGAME_CARDMOVING_H

#include "../Matrix/SquareMatrix.h"
#include "../Model/Cards/Card.h"
#include "../Vector/Float3.h"

class CardMoving {
   Card* card;
   float angle;
   double totalDuration;

   Float3 dir;
   Float3 height;

public:
   CardMoving(Card* card, double totalDuration);

   void moveCard(const Float3& direction, double duration);

    Card* getCard() const;
};

#endif //COTECCHIOGAME_CARDMOVING_H
