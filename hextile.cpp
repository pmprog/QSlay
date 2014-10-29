#include "hextile.h"

HexTile::HexTile()
{
	Owner = -1;
	HasHouse = false;
	HasCastle = false;
	HasMan = false;
}

void HexTile::Reset()
{
	Owner = -1;
	HasHouse = false;
	HasCastle = false;
	HasMan = false;
}
