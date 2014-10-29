#include "hextile.h"

int HexTile::ManCosts[] = { 2, 6, 18, 54 };

HexTile::HexTile()
{
	Reset();
}

void HexTile::Reset()
{
	Owner = -1;
	HasHouse = false;
	HasCastle = false;
	HasMan = false;
	HasTree = false;
	HasGrave = false;
}
