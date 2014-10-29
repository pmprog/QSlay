#ifndef HEXMAP_H
#define HEXMAP_H

#include "qtlibs.h"
#include "hextile.h"

#define MAP_TILE_EDGING	 7
#define MAP_TILE_WIDTH	30
#define MAP_TILE_HEIGHT	24
#define MAP_TILE_SHADOW	 3
#define MAP_MAX_WIDTH		30
#define MAP_MAX_HEIGHT	14

#define MAP_SIZE_SMALL	 0
#define MAP_SIZE_MEDIUM	 1
#define MAP_SIZE_LARGE	 2
#define MAP_SIZE_XLARGE	 3

class HexMap : public QGraphicsItem
{
	private:
		QPixmap* SpriteSheet;
		int SpriteFrame;
		int LastSeed;

		void GetGroupedTilesRecurse(int X, int Y, QList<QPoint>* List);

	public:
		HexTile* Tiles[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];	// 32 x 24 grid

		HexMap( unsigned int Seed );

		void Animate();

		QRectF boundingRect() const;
		QPainterPath shape() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

		QPolygon TileToPolygon(int X, int Y, bool Shadow);
		QPoint TileSpritePosition(int X, int Y);

		void GenerateMap(unsigned int Seed, int NumberOfPlayers, int MapSize);
		QList<QPoint> GetAdjectentTiles(int X, int Y);
		QList<QPoint> GetAdjectentOwnedTiles(int X, int Y, int Owner);
		QList<QPoint> GetGroupedTiles(int X, int Y);

		void CombineHouses();


};

#endif // HEXMAP_H
