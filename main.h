#ifndef MAIN_H
#define MAIN_H

#include "qtlibs.h"
#include "hexmap.h"
#include "game.h"


QApplication* appObj;
QGraphicsScene* sceneObj;
QGraphicsView* viewObj;

QPixmap* CardBackImage;
QPixmap* CardImages[52];

HexMap* Map;
Game* CurrentGame;

#endif // MAIN_H
