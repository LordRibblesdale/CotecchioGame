#include "Player.h"

Player::Player(unsigned short nCards, Float3 position) : position(std::move(position)) {
   cards.reserve(nCards);
}

std::vector <Card> &Player::getCards() {
   return cards;
}

const Float3 &Player::getPosition() const {
   return position;
}

