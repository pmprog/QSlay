#include "game.h"

extern HexMap* Map;

Game::Game(QObject *parent) :
    QObject(parent)
{
}

void Game::Animate()
{
	Map->Animate();
}
