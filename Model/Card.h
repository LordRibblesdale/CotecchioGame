#ifndef COTECCHIOGAME_CARD_H
#define COTECCHIOGAME_CARD_H

#include <iostream>
#include "../Matrix/SquareMatrix.h"

extern float cardVertices[32];
extern unsigned int vIndices[6];

struct Card {
   unsigned short int playerID;
   unsigned int playerCardNum;

   unsigned int value;
   float u = 0,  v = 0;

   // value è del tipo NM (10, 11, 20, 39, ....)
   Card(unsigned int vaule, unsigned short int playerID, unsigned int playerCardNum) {
      Card::value = vaule;
      Card::playerID = playerID;
      Card::playerCardNum = playerCardNum;

      u = (value%10)*0.1f;
      v = ((value/10)-1)*0.25f;
   }

   SquareMatrix getWorldCoordinates() const;

   void updateCoords() const;
};

#endif //COTECCHIOGAME_CARD_H
