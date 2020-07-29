#ifndef COTECCHIOGAME_CARD_H
#define COTECCHIOGAME_CARD_H

#include <iostream>
#include <vector>
#include "../../Matrix/SquareMatrix.h"
#include "../../Vector/Float4.h"
#include "../../Matrix/StandardMatrix.h"

extern float cardVertices[32];
extern unsigned int vIndices[6];

struct Card {
   unsigned short int playerID;

   unsigned int value;

   SquareMatrix local2World;
   std::unique_ptr<SquareMatrix> hand2Table;
   std::unique_ptr<SquareMatrix> rotationOnTable;

   Float3         tableCenterToHand;   // Vettore distanziatore tra il centro del tavolo e le carte
   Float3         orthogonalTC2H;      // Vettore ortogonale al vettore distanziatore
   SquareMatrix   translate;           // Matrice di traslazione di ogni carta
   SquareMatrix   qRotate;             // Matrice di rotazione della carta in mano
   SquareMatrix   rotate;              // Matrice di rotazione della mano rispetto al giocatore
   SquareMatrix   scale;               // Matrice di scala delle carte

   SquareMatrix partialLocal2World;

   float angle = degree2Radiants(-10);

   bool isSelected;
   bool isCardSetup = false;

   size_t handCards;

   // value è del tipo NM (10, 11, 20, 39, ....)
   Card(unsigned int vaule, unsigned short int playerID);
   Card(const Card& card);
   ~Card();

   Card& operator=(const Card& card);

   SquareMatrix getWorldCoordinates(unsigned int cardIndex);
   void getWorldCoordinates(unsigned int cardIndex, std::vector<SquareMatrix>& matrices);

    void setupCard();

   const SquareMatrix& getLocal2World() const;

   Float4 getWorldCardCenter();

   bool isSelected1() const;

   void setIsSelected(bool isSelected);
};

#endif //COTECCHIOGAME_CARD_H
