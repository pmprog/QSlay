#ifndef HEXTILE_H
#define HEXTILE_H

class HexTile
{
	public:
		static int ManCosts[];

		int Owner;
		bool HasHouse;
		bool HasCastle;
		bool HasMan;
		bool HasTree;
		bool HasGrave;

		int HouseCash;

		int ManType;
		bool ManMoved;

		int TreeType;

		HexTile();

		void Reset();
};

#endif // HEXTILE_H
