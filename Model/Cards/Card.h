#ifndef COTECCHIOGAME_CARD_H
#define COTECCHIOGAME_CARD_H

#include <iostream>
#include <vector>
#include "../../Matrix/SquareMatrix.h"
#include "../../Vector/Float4.h"
#include "../../Matrix/StandardMatrix.h"

extern float cardVertices[24];
extern float backUVArray[8];
extern unsigned int vIndices[6];

struct Card {
   unsigned short int playerID;

   unsigned int value;
   float u = 0,  v = 0;

   float cardUVArray[8];

   SquareMatrix local2World;
   std::unique_ptr<SquareMatrix> hand2Table;

   Float3 t;                  // Vettore distanziatore tra il centro del tavolo e le carte
   Float3 tmp;                // Vettore ortogonale al vettore distanziatore
   SquareMatrix translate;    // Matrice di traslazione di ogni carta
   SquareMatrix qRotate;      // Matrice di rotazione della carta in mano
   SquareMatrix rotate;       // Matrice di rotazione della mano rispetto al giocatore
   SquareMatrix scale;        // Matrice di scala delle carte

   SquareMatrix partialLocal2World;

   float angle = degree2Radiants(-10);

   bool isSelected;
   bool isCardSetup = false;

   size_t handCards;

   // value è del tipo NM (10, 11, 20, 39, ....)
   Card(unsigned int vaule, unsigned short int playerID);
   // TODO check if std::moving member from vector to move ctor destroys elements in vector (removes automatically)
   Card(const Card& card);
   ~Card();

   Card& operator=(const Card& card);

   SquareMatrix getWorldCoordinates(unsigned int cardIndex);
   void getWorldCoordinates(unsigned int cardIndex, std::vector<SquareMatrix>& matrices);

    void setupCard();

   // Scritta appositamente per crearla come copia
   const SquareMatrix& getLocal2World() const;

   Float4 getWorldCardCenter();

   bool isSelected1() const;

   void setIsSelected(bool isSelected);
};

#endif //COTECCHIOGAME_CARD_H
