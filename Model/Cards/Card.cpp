#include "Card.h"
#include "../../Matrix/StandardMatrix.h"
#include "../../Graphics/SceneObjects.h"

/*
float cardVertices[32] {
        -0.5f, 0, -0.825f, 0, 0, 0, 1, 0,
        0.5f, 0, -0.825f, 1, 0, 0, 1, 0,
        -0.5f, 0, 0.825f, 0, 1, 0, 1, 0,
        0.5f, 0, 0.825f, 1, 1, 0, 1, 0,
};
 */

float cardVertices[24] {
        -0.5f, 0, -0.825f, 0, 1, 0,
        0.5f, 0, -0.825f, 0, 1, 0,
        -0.5f, 0, 0.825f, 0, 1, 0,
        0.5f, 0, 0.825f, 0, 1, 0
};

float cardUVArray[8] {
        0, 0,
        1, 0,
        0, 1,
        1, 1
};

unsigned int vIndices[6] {
        0, 1, 2, 2, 1, 3
};

/* Mazzo di carte
   float angle = degree2Radiants(-45);
   unsigned int handCards = players.at(playerID).getCards().size();
   Float3 t(std::move(players.at(playerID).getPosition() - 3*(players.at(playerID).getPosition() - lookAt).getNormalized()));
   Float3 tmp(-t.getY()*0.2f - 0.1f*cardIndex / static_cast<float>(handCards), t.getX()*0.2f + 0.4f * (handCards%2 != 0), 0);
   SquareMatrix translate(std::move(Transform::translateMatrix4(
           t.getX() - tmp.getX(),//*(1 - 2*cardIndex / static_cast<float>(handCards)),
           t.getY() - tmp.getY(),// * (1 - 2*cardIndex / static_cast<float>(handCards)),
           t.getZ())));
   SquareMatrix qRotate(std::move(Rotation::rotationByQuaternion(Float4(t.getX(), t.getY(), 0, 1), angle * (1 + 2*M_PI*cardIndex / static_cast<float>(handCards)))));
   SquareMatrix rotate(std::move(Rotation::rotationZAxisMatrix4(M_PI_2 - ((M_PI/static_cast<float>(sessionPlayers))*(sessionPlayers-2)*playerID))));
   SquareMatrix scale(std::move(Transform::scaleMatrix4(0.4, 0.4, 0.4)));

   //local2World = std::move((translate * (qRotate * (rotate * scale))));
   local2World = std::move((translate * (rotate * scale)));

   return local2World;
 */

SquareMatrix Card::getWorldCoordinates(unsigned int cardIndex) {
   // TODO optimize memory & operations
   unsigned int handCards = players.at(playerID).getCards().size();
   Float3 t(std::move(players.at(playerID).getPosition() - 4.0f*(players.at(playerID).getPosition() - lookAt).getNormalized()));
   Float3 tmp(-t.getY()*0.2f, t.getX()*0.2f, 0);
   SquareMatrix translate(std::move(Transform::translateMatrix4(
           t.getX() - tmp.getX() * (1 - 3.25f*cardIndex / static_cast<float>(handCards)),
           t.getY() - tmp.getY() * (1 - 3.25f*cardIndex / static_cast<float>(handCards)),
           t.getZ() - 0.5f)));
   SquareMatrix qRotate(std::move(Rotation::rotationByQuaternion(Float4(tmp.getX(), tmp.getY(), 0, 1), degree2Radiants(-10))));
   SquareMatrix rotate(std::move(Rotation::rotationZAxisMatrix4(M_PI_2 - ((M_PI/static_cast<float>(sessionPlayers))*(sessionPlayers-2)*playerID))));
   SquareMatrix scale(std::move(Transform::scaleMatrix4(0.4, 0.4, 0.4)));

   local2World = std::move((translate * (qRotate * (rotate * scale))));

   return local2World;
}

void Card::getWorldCoordinates(unsigned int cardIndex, std::vector<SquareMatrix>& matrices) {
   unsigned int handCards = players.at(playerID).getCards().size();
   Float3 t(std::move(players.at(playerID).getPosition() - 2.5f*(players.at(playerID).getPosition() - lookAt).getNormalized()));
   Float3 tmp(-t.getY()*0.2f - 0.1f*cardIndex / static_cast<float>(handCards), t.getX()*0.2f, 0);
   matrices.emplace_back(std::move(Transform::translateMatrix4(
           t.getX() - tmp.getX() * (1 - 2.25f*cardIndex / static_cast<float>(handCards)),
           t.getY() - tmp.getY() * (1 - 2.25f*cardIndex / static_cast<float>(handCards)),
           t.getZ() - 0.5f)));
   matrices.emplace_back(std::move(Rotation::rotationByQuaternion(Float4(tmp.getX(), tmp.getY(), 0, 1), degree2Radiants(-10))));
   matrices.emplace_back(std::move(Rotation::rotationZAxisMatrix4(M_PI_2 - ((M_PI/static_cast<float>(sessionPlayers))*(sessionPlayers-2)*playerID))));
   matrices.emplace_back(std::move(Transform::scaleMatrix4(0.4, 0.4, 0.4)));
}

void Card::updateCoords() const {
   // Optimize here;
   cardUVArray[0] = u;
   cardUVArray[1] = v;
   cardUVArray[2] = u + 0.1f;
   cardUVArray[3] = v;
   cardUVArray[4] = u;
   cardUVArray[5] = v + 0.25f;
   cardUVArray[6] = u + 0.1f;
   cardUVArray[7] = v + 0.25f;
}

SquareMatrix Card::getLocal2World() const {
   return local2World;
}

bool Card::isSelected1() const {
   return isSelected;
}

void Card::setIsSelected(bool isSelected) {
   Card::isSelected = isSelected;
}


