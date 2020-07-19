#include "CardMoving.h"
#include "../../Graphics/SceneObjects.h"

CardMoving::CardMoving(Card* card, double totalDuration) : card(card), totalDuration(totalDuration) {
   angle = card->angle;

   height = std::move(Float3(0, 0, maxZ));
   dir = std::move(card->t - height);
}

void CardMoving::moveCard(const Float3& direction, double duration) {
   double t = duration / totalDuration;
   Float3 tmpDir(-t*t * dir);
   SquareMatrix translation(std::move(Transform::translateMatrix4(tmpDir.getX(), tmpDir.getY(), tmpDir.getZ())));

   //card->hand2Table = std::move(std::make_unique<SquareMatrix>(/*translation * */Rotation::rotationByQuaternion(translation.multiplyVector(Float4(card->tmp, false)), angle* duration)));
   /*card->hand2Table = std::move(std::make_unique<SquareMatrix>(
           Rotation::rotationYAxisMatrix4(-angle*t*t) * Rotation::rotationXAxisMatrix4()));
   */
   //std::cout << card->hand2Table->toString() << std::endl;
}

Card* CardMoving::getCard() const {
   return card;
}
