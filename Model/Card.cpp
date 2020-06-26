#include "Card.h"
#include "../Matrix/StandardMatrix.h"
#include "../Graphics/SceneObjects.h"

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

SquareMatrix Card::getWorldCoordinates(unsigned int cardIndex) const {
   // TODO optimize memory & operations
   float angle = degree2Radiants(-45);
   float handCards = players.at(playerID).getCards().size();
   Float3 t(std::move(players.at(playerID).getPosition() - 3*(players.at(playerID).getPosition() - lookAt).getNormalized()));
   Float3 tmp(-t.getY()*0.2f - 0.5f*cardIndex / static_cast<float>(handCards), t.getX()*0.2f, 0);
   SquareMatrix translate(std::move(Transform::translateMatrix4(
           t.getX() - tmp.getX()*(1 - 2*cardIndex / static_cast<float>(handCards)),
           t.getY() - tmp.getY() * (1 - 2*cardIndex / static_cast<float>(handCards)),
           t.getZ())));
   SquareMatrix qRotate(std::move(Rotation::rotationByQuaternion(Float4(t.getX(), t.getY(), 0, 1), angle * (1 + 2*M_PI*cardIndex / static_cast<float>(handCards)))));
   SquareMatrix rotate(std::move(Rotation::rotationZAxisMatrix4(M_PI_2 - ((M_PI/static_cast<float>(sessionPlayers))*(sessionPlayers-2)*playerID))));
   SquareMatrix scale(std::move(Transform::scaleMatrix4(0.4, 0.4, 0.4)));

   return (translate * (qRotate * (rotate * scale)));
}

void Card::updateCoords() const {
   //TODO optimize here;
   cardUVArray[0] = u;
   cardUVArray[1] = v;
   cardUVArray[2] = u + 0.1f;
   cardUVArray[3] = v;
   cardUVArray[4] = u;
   cardUVArray[5] = v + 0.25f;
   cardUVArray[6] = u + 0.1f;
   cardUVArray[7] = v + 0.25f;
}


