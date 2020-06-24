#include "Card.h"

#include <cmath>
#include "../Matrix/StandardMatrix.h"
#include "../Graphics/SceneObjects.h"

float cardVertices[32] {
        -0.5f, 0, -0.825f, 0, 0, 0, 1, 0,
        0.5f, 0, -0.825f, 1, 0, 0, 1, 0,
        -0.5f, 0, 0.825f, 0, 1, 0, 1, 0,
        0.5f, 0, 0.825f, 1, 1, 0, 1, 0,
};

unsigned int vIndices[6] {
        0, 1, 2, 2, 1, 3
};

SquareMatrix Card::getWorldCoordinates() const {
   // TODO optimize memory & operations
   float angle = degree2Radiants(45);
   Float3 t(std::move(players.at(playerID).getPosition() + (players.at(playerID).getPosition() - lookAt).getNormalized()));
   SquareMatrix translate(std::move(Transform::translateMatrix4(t.getX(), t.getY(), t.getZ())));
   SquareMatrix rotate(std::move(Rotation::rotationByQuaternion(Float4(t, true), -angle + (angle/(2.0f*players.at(playerID).getCards().size()))*playerCardNum)));
   SquareMatrix scale(std::move(Transform::scaleMatrix4(0.25, 0.25, 0.25)));

   return (translate * (rotate * scale));
}

void Card::updateCoords() const {
   cardVertices[3] = u;
   cardVertices[4] = v;
   cardVertices[8 + 3] = u + 0.1f;
   cardVertices[8 + 4] = v;
   cardVertices[16 + 3] = u;
   cardVertices[16 + 4] = v + 0.25f;
   cardVertices[24 + 3] = u + 0.1f;
   cardVertices[24 + 4] = v + 0.25f;
}


