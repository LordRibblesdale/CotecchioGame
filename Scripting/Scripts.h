#ifndef COTECCHIOGAME_SCRIPTS_H
#define COTECCHIOGAME_SCRIPTS_H

#include "../Graphics/SceneObjects.h"

void increaseIndex();

void createPlayerPositions(unsigned short int nPlayers);

void pollInput(GLFWwindow *window);

void cursorPositionCallBack(GLFWwindow* window, double xPos, double yPos);

void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset);

void refreshWindowSize(GLFWwindow *window, int width, int height);

#endif //COTECCHIOGAME_SCRIPTS_H
