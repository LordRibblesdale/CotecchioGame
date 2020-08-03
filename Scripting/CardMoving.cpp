#include "CardMoving.h"
#include "../../Graphics/SceneObjects.h"

float CardMoving::heightBias = 0;

CardMoving::CardMoving(Card* card, double totalDuration) : card(card), totalDuration(totalDuration) {
   angle = card->angle;

   dir = std::move(Float3(card->tableCenterToHand.getX(), card->tableCenterToHand.getY(), card->tableCenterToHand.getZ() - maxZ));
}

void CardMoving::moveCard(const Float3& direction, double duration) const {
   double t = duration / totalDuration;
   // Traslazione dalla mano al centro del tavolo (trasla pian piano che aumenta t, ovvero aumenta il tempo)
   Float3 tmpDir(-t*t * dir);
   // Matrice per la traslazione, in dipendenza dal fattore t
   SquareMatrix translation(std::move(Transform::translateMatrix4(tmpDir.getX(), tmpDir.getY(), tmpDir.getZ() + t*0.3f + heightBias)));
   // Rotazione nel Local Space rispetto a X (carta parallela a X nel cardVertices)
   SquareMatrix rotationX(std::move(Rotation::rotationXAxisMatrix4(((playerIndex % 2 == 0) ? -1.0f : 1.0f) * t*t * (M_PI_2 + angle))));

   card->hand2Table = std::move(std::make_unique<SquareMatrix>(std::move(translation)));
   card->rotationOnTable = std::move(std::make_unique<SquareMatrix>(std::move(rotationX)));
}

Card* CardMoving::getCard() const {
   return card;
}
