#ifndef COTECCHIOGAME_PLAYER_H
#define COTECCHIOGAME_PLAYER_H

#include <vector>
#include "../Model/Card.h"
#include "../Vector/Float3.h"

class Player {
   std::vector<Card> cards;

   Float3 position;

public:
   Player(unsigned short int nCards, Float3 position);

   std::vector<Card> &getCards();
   const Float3 &getPosition() const;
};


#endif //COTECCHIOGAME_PLAYER_H
