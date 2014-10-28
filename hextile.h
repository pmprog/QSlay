#ifndef HEXTILE_H
#define HEXTILE_H

class HexTile
{
	public:
		int Owner;
		bool HasHouse;
		bool HasCastle;
		bool HasMan;

		int HouseCash;

		int ManType;
		bool ManMoved;

		HexTile();

};

#endif // HEXTILE_H
