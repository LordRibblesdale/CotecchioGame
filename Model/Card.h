#ifndef COTECCHIOGAME_CARD_H
#define COTECCHIOGAME_CARD_H

struct Card {
   float cardVertices[32] {
      -0.5f, 0, -0.825f, 0, 0, 0, 1, 0,
      0.5f, 0, -0.825f, 1, 0, 0, 1, 0,
      -0.5f, 0, 0.825f, 0, 1, 0, 1, 0,
      0.5f, 0, 0.825f, 1, 1, 0, 1, 0,
   };

   unsigned int vIndices[6] {
      0, 1, 2, 2, 1, 3
   };
};

#endif //COTECCHIOGAME_CARD_H
