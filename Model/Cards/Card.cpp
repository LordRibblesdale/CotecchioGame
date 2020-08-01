#include "Card.h"
#include "../../Graphics/SceneObjects.h"

float cardVertices[32] {
        -0.5f, 0, -0.825f, 0, 0, 0, 1, 0,
        0.5f, 0, -0.825f, 1, 0, 0, 1, 0,
        -0.5f, 0, 0.825f, 0, 1, 0, 1, 0,
        0.5f, 0, 0.825f, 1, 1, 0, 1, 0
};

unsigned int vIndices[6] {
        0, 1, 2, 2, 1, 3
};

Card::Card(unsigned int vaule, unsigned short playerID)
        : local2World(std::move(SquareMatrix(4, {}))), translate(std::move(SquareMatrix(4, {}))),
          qRotate(std::move(SquareMatrix(4, {}))), rotate(std::move(SquareMatrix(4, {}))),
          scale(std::move(SquareMatrix(4, {}))), partialLocal2World(std::move(SquareMatrix(4, {}))) {
   Card::value = vaule;
   Card::playerID = playerID;

   scale = std::move(Transform::scaleMatrix4(0.4, 0.4, 0.4));
   hand2Table = std::move(std::make_unique<SquareMatrix>(std::move(Transform::scaleMatrix4(1, 1, 1))));
   rotationOnTable = std::move(std::make_unique<SquareMatrix>(std::move(Transform::scaleMatrix4(1, 1, 1))));
}

Card::Card(const Card &card) : local2World(card.local2World), translate(card.translate), qRotate(card.qRotate),
                                 rotate(card.rotate), scale(card.scale), partialLocal2World(card.partialLocal2World),
                                 value(card.value), playerID(card.playerID) {
   Card::scale = card.scale;
   Card::tableCenterToHand = card.tableCenterToHand;
   Card::orthogonalTC2H = card.orthogonalTC2H;

   if (!card.hand2Table) {
      hand2Table = std::move(std::make_unique<SquareMatrix>(std::move(Transform::scaleMatrix4(1, 1, 1))));
   } else {
      hand2Table = std::move(std::make_unique<SquareMatrix>(*card.hand2Table));
   }

   if (!card.rotationOnTable) {
      rotationOnTable = std::move(std::make_unique<SquareMatrix>(std::move(Transform::scaleMatrix4(1, 1, 1))));
   } else {
      rotationOnTable = std::move(std::make_unique<SquareMatrix>(*card.rotationOnTable));
   }
}

Card& Card::operator=(const Card &card) {
   Card::value = card.value;
   Card::playerID = card.playerID;
   Card::tableCenterToHand = card.tableCenterToHand;
   Card::orthogonalTC2H = card.orthogonalTC2H;

   Card::scale = card.scale;
   Card::local2World = card.local2World;
   Card::translate = card.translate;
   Card::qRotate = card.qRotate;
   Card::rotate = card.rotate;
   Card::partialLocal2World = card.partialLocal2World;

   if (!card.hand2Table) {
      hand2Table = std::move(std::make_unique<SquareMatrix>(std::move(Transform::scaleMatrix4(1, 1, 1))));
   } else {
      hand2Table = std::move(std::make_unique<SquareMatrix>(*card.hand2Table));
   }

   if (!card.rotationOnTable) {
      rotationOnTable = std::move(std::make_unique<SquareMatrix>(std::move(Transform::scaleMatrix4(1, 1, 1))));
   } else {
      rotationOnTable = std::move(std::make_unique<SquareMatrix>(*card.rotationOnTable));
   }

   return *this;
}

Card::~Card() {
   playerID = 0;
   value = 0;
   angle = 0;
   handCards = 0;

   hand2Table.reset();
   rotationOnTable.reset();
}

/* Mazzo di carte
   float angle = degree2Radiants(-45);
   unsigned int handCards = players.at(playerID).getCards().size();
   Float3 tableCenterToHand(std::move(players.at(playerID).getPosition() - 3*(players.at(playerID).getPosition() - defaultLookAt).getNormalized()));
   Float3 orthogonalTC2H(-tableCenterToHand.getY()*0.2f - 0.1f*cardIndex / static_cast<float>(handCards), tableCenterToHand.getX()*0.2f + 0.4f * (handCards%2 != 0), 0);
   SquareMatrix translate(std::move(Transform::translateMatrix4(
           tableCenterToHand.getX() - orthogonalTC2H.getX(),//*(1 - 2*cardIndex / static_cast<float>(handCards)),
           tableCenterToHand.getY() - orthogonalTC2H.getY(),// * (1 - 2*cardIndex / static_cast<float>(handCards)),
           tableCenterToHand.getZ())));
   SquareMatrix qRotate(std::move(Rotation::rotationByQuaternion(Float4(tableCenterToHand.getX(), tableCenterToHand.getY(), 0, 1), angle * (1 + 2*M_PI*cardIndex / static_cast<float>(handCards)))));
   SquareMatrix rotate(std::move(Rotation::rotationZAxisMatrix4(M_PI_2 - ((M_PI/static_cast<float>(sessionPlayers))*(sessionPlayers-2)*playerID))));
   SquareMatrix scale(std::move(Transform::scaleMatrix4(0.4, 0.4, 0.4)));

   //local2World = std::move((translate * (qRotate * (rotate * scale))));
   local2World = std::move((translate * (rotate * scale)));

   return local2World;
 */

void Card::setupCard() {
   handCards = players.at(playerID).getCards().size();

   tableCenterToHand = std::move(players.at(playerID).getPosition() - 4.0f * (players.at(playerID).getPosition() - defaultLookAt).getNormalized());
   orthogonalTC2H = std::move(Float3(-tableCenterToHand.getY() * 0.2f + 0.001f, tableCenterToHand.getX() * 0.2f + 0.001f, 0));
   rotate = std::move(Rotation::rotationZAxisMatrix4(M_PI_2 - ((M_PI/static_cast<float>(sessionPlayers))*(sessionPlayers-2)*playerID)));
   qRotate = std::move(Rotation::rotationByQuaternion(Float3(orthogonalTC2H.getX(), orthogonalTC2H.getY(), 0), angle));

   partialLocal2World = std::move((qRotate * (rotate * scale)));
}

SquareMatrix Card::getWorldCoordinates(unsigned int cardIndex) {
   if (!isCardSetup) {
      setupCard();

      isCardSetup = true;
   }

   translate = (std::move(Transform::translateMatrix4(
         tableCenterToHand.getX() - orthogonalTC2H.getX() * (1.75f - 3.25f * (cardIndex + 1) / static_cast<float>(handCards)) - 0.10f,
         tableCenterToHand.getY() - orthogonalTC2H.getY() * (1.0f - 3.25f * (cardIndex) / static_cast<float>(handCards)) - 0.10f,
         tableCenterToHand.getZ() - 0.4f)));

   local2World = std::move(translate * partialLocal2World);

   return local2World;
}

void Card::getWorldCoordinates(unsigned int cardIndex, SquareMatrix& matrix) {
   if (!isCardSetup) {
      setupCard();

      isCardSetup = true;
   }

   translate = (std::move(Transform::translateMatrix4(
         tableCenterToHand.getX() - orthogonalTC2H.getX() * (1.75f - 3.25f * (cardIndex + 1) / static_cast<float>(handCards)) - 0.10f,
         tableCenterToHand.getY() - orthogonalTC2H.getY() * (1.0f - 3.25f * (cardIndex) / static_cast<float>(handCards)) - 0.10f,
         tableCenterToHand.getZ() - 0.4f)));

   local2World = std::move(translate * partialLocal2World);

   matrix = local2World;
}

const SquareMatrix& Card::getLocal2World() const {
   return local2World;
}

Float4 Card::getWorldCardCenter() {
   return local2World.multiplyVector(Float4(0, 0, 0, 1));
}

bool Card::isSelected1() const {
   return isSelected;
}

void Card::setIsSelected(bool isSelected) {
   Card::isSelected = isSelected;
}