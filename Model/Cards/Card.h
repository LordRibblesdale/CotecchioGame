#ifndef COTECCHIOGAME_CARD_H
#define COTECCHIOGAME_CARD_H

#include <iostream>
#include <vector>
#include "../../Matrix/SquareMatrix.h"

extern float cardVertices[24];
extern float cardUVArray[8];
extern unsigned int vIndices[6];

struct Card {
   unsigned short int playerID;

   unsigned int value;
   float u = 0,  v = 0;

   SquareMatrix local2World;

   bool isSelected;

   // value è del tipo NM (10, 11, 20, 39, ....)
   Card(unsigned int vaule, unsigned short int playerID) : local2World(std::move(SquareMatrix(4, {}))) {
      Card::value = vaule;
      Card::playerID = playerID;

      u = (value%10)*0.1f;
      v = ((value/10)-1)*0.25f;
   }

   SquareMatrix getWorldCoordinates(unsigned int cardIndex);
   void getWorldCoordinates(unsigned int cardIndex, std::vector<SquareMatrix>& matrices);

   void updateCoords() const;

   // Scritta appositamente per crearla come copia
   SquareMatrix getLocal2World() const;

   bool isSelected1() const;

   void setIsSelected(bool isSelected);
};

#endif //COTECCHIOGAME_CARD_H
