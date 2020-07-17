#include "CardMoving.h"

CardMoving::CardMoving(Card* card, double totalDuration) : card(card), totalDuration(totalDuration) {
   center = std::move(card->getWorldCardCenter());
   angle = card->angle;
}

void CardMoving::moveCard(const Float3& direction, double duration) {
   double t = duration / totalDuration;
   Float4 tmp((1-t)* center);

   card->hand2Table = std::move(std::make_unique<SquareMatrix>(Transform::translateMatrix4(tmp.getX(), tmp.getY(), tmp.getZ()) * Rotation::rotationByQuaternion(Float4(direction, false), angle* duration)));
}

Card* CardMoving::getCard() const {
   return card;
}
