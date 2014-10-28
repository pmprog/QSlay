#include "hexmap.h"




HexMap::HexMap( unsigned int Seed )
{
	LastSeed = Seed;
	srand( Seed );
	SpriteSheet = new QPixmap(":/sprites.png");

	for( int x = 0; x < MAP_MAX_WIDTH; x++ )
	{
		for( int y = 0; y < MAP_MAX_HEIGHT; y++ )
		{
			Tiles[x][y] = new HexTile();
		}
	}

	SpriteFrame = 0;
}

void HexMap::Animate()
{
	SpriteFrame = (SpriteFrame + 1) % 4;
	this->scene()->invalidate();
}

QRectF HexMap::boundingRect() const
{
	return QRectF( 0, 0, (MAP_MAX_WIDTH * (MAP_TILE_WIDTH - MAP_TILE_EDGING)) + MAP_TILE_EDGING, MAP_MAX_HEIGHT * (MAP_TILE_HEIGHT + 1) );
}

QPainterPath HexMap::shape() const
{
	QPainterPath path;
	path.addRect( boundingRect() );
	return path;
}

void HexMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen( Qt::black );
	for( int x = 0; x < MAP_MAX_WIDTH; x++ )
	{
		for( int y = 0; y < MAP_MAX_HEIGHT; y++ )
		{
			if( Tiles[x][y]->Owner >= 0 )
			{
				painter->setBrush( Qt::black );
				painter->drawPolygon( TileToPolygon(x, y, true), Qt::OddEvenFill );

				switch( Tiles[x][y]->Owner )
				{
					case 0:
					painter->setBrush( QColor( 0, 255, 0 ) );
						break;
					case 1:
						painter->setBrush( QColor( 192, 192, 0 ) );
						break;
					case 2:
						painter->setBrush( QColor( 0, 128, 0 ) );
						break;
					case 3:
						painter->setBrush( QColor( 255, 255, 0 ) );
						break;
					case 4:
						painter->setBrush( QColor( 0, 192, 0 ) );
						break;
					case 5:
						painter->setBrush( QColor( 128, 128, 0 ) );
						break;
				}
				painter->drawPolygon( TileToPolygon(x, y, false), Qt::OddEvenFill );

				if( Tiles[x][y]->HasCastle )
				{
					painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(115, 1, 18, 28) );
				}
				if( Tiles[x][y]->HasHouse )
				{
					if( Tiles[x][y]->HouseCash < 10 )
					{
						painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(96, 1, 18, 28) );
					} else {
						painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(77, (SpriteFrame * 29) + 1, 18, 28) );
					}
				}

				if( Tiles[x][y]->HasMan )
				{
					int manx = (Tiles[x][y]->ManType * 19) + 1;
					if( Tiles[x][y]->ManMoved )
					{
						painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(manx, 1, 18, 28) );
					} else {
						painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(manx, (SpriteFrame * 29) + 1, 18, 28) );
					}
				}

			}
		}
	}
}

void HexMap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	GenerateMap(LastSeed + 1, 6, MAP_SIZE_MEDIUM);
}

void HexMap::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /*
    if( event->buttons() == Qt::LeftButton )
    {
        this->setPos( this->x() + (event->pos().x() - event->buttonDownPos( Qt::LeftButton ).x()), this->y() + (event->pos().y() - event->buttonDownPos( Qt::LeftButton ).y()) );
    }
    */
}

QPolygon HexMap::TileToPolygon(int X, int Y, bool Shadow)
{
	QPolygon q(0);

	int coreX = X * (MAP_TILE_WIDTH - MAP_TILE_EDGING);
	int coreY = Y * MAP_TILE_HEIGHT;
	if( X % 2 == 1 )
	{
		coreY += (MAP_TILE_HEIGHT / 2);
	}

	q.append( QPoint(coreX + MAP_TILE_EDGING + (Shadow ? MAP_TILE_SHADOW : 0), coreY + (Shadow ? MAP_TILE_SHADOW : 0)) );
	q.append( QPoint(coreX + MAP_TILE_WIDTH - MAP_TILE_EDGING + (Shadow ? MAP_TILE_SHADOW : 0), coreY + (Shadow ? MAP_TILE_SHADOW : 0)) );
	q.append( QPoint(coreX + MAP_TILE_WIDTH + (Shadow ? MAP_TILE_SHADOW : 0), coreY + (MAP_TILE_HEIGHT / 2) + 1 + (Shadow ? MAP_TILE_SHADOW : 0)) );
	q.append( QPoint(coreX + MAP_TILE_WIDTH - MAP_TILE_EDGING + (Shadow ? MAP_TILE_SHADOW : 0), coreY + MAP_TILE_HEIGHT + (Shadow ? MAP_TILE_SHADOW : 0)) );
	q.append( QPoint(coreX + MAP_TILE_EDGING + (Shadow ? MAP_TILE_SHADOW : 0), coreY + MAP_TILE_HEIGHT + (Shadow ? MAP_TILE_SHADOW : 0)) );
	q.append( QPoint(coreX + (Shadow ? MAP_TILE_SHADOW : 0), coreY + (MAP_TILE_HEIGHT / 2) + 1 + (Shadow ? MAP_TILE_SHADOW : 0)) );

	return q;
}

QPoint HexMap::TileSpritePosition(int X, int Y)
{
	int coreX = X * (MAP_TILE_WIDTH - MAP_TILE_EDGING);
	int coreY = Y * MAP_TILE_HEIGHT;
	if( X % 2 == 1 )
	{
		coreY += (MAP_TILE_HEIGHT / 2);
	}

	return QPoint( coreX + (MAP_TILE_WIDTH / 2) - 9, coreY + ((MAP_TILE_HEIGHT / 4) * 3) - 28 );
}

void HexMap::GenerateMap(unsigned int Seed, int NumberOfPlayers, int MapSize)
{
	LastSeed = Seed;
	srand( Seed );

	for( int x = 0; x < MAP_MAX_WIDTH; x++ )
	{
		for( int y = 0; y < MAP_MAX_HEIGHT; y++ )
		{
			Tiles[x][y]->Owner = -1;
		}
	}

	int tilesToGo;
	switch( MapSize )
	{
		case MAP_SIZE_SMALL:
			tilesToGo = 80 + (rand() % 24);
			break;
		case MAP_SIZE_MEDIUM:
			tilesToGo = 130 + (rand() % 22);
			break;
		case MAP_SIZE_LARGE:
			tilesToGo = 160 + (rand() % 18);
			break;
		case MAP_SIZE_XLARGE:
			tilesToGo = 220 + (rand() % 20);
			break;
	}

	int x = (rand() % (MAP_MAX_WIDTH / 3)) + (MAP_MAX_WIDTH / 3);
	int y = rand() % MAP_MAX_HEIGHT;

	while( tilesToGo > 0 )
	{
		if( Tiles[x][y]->Owner == - 1 )
		{
			Tiles[x][y]->Owner = (abs(rand()) % NumberOfPlayers);
			tilesToGo--;
		}
		QList<QPoint> adj = GetAdjectentTiles(x, y);
		int r = rand() % adj.count();
		x = adj[r].x();
		y = adj[r].y();
	}

}

QList<QPoint> HexMap::GetAdjectentTiles(int X, int Y)
{
	QList<QPoint> l;

	if( Y > 0 )
	{
		l.append( QPoint( X, Y - 1 ));
	}
	if( X % 2 == 1 )
	{

		if( X < MAP_MAX_WIDTH - 1 )
		{
			l.append( QPoint( X + 1, Y ));
			if( Y < MAP_MAX_HEIGHT - 1 )
			{
				l.append( QPoint( X + 1, Y + 1 ));
			}
		}
		if( Y < MAP_MAX_HEIGHT - 1 )
		{
			l.append( QPoint( X, Y + 1 ));
		}
		if( X > 0 )
		{
			if( Y < MAP_MAX_HEIGHT - 1 )
			{
				l.append( QPoint( X - 1, Y + 1 ));
			}
			l.append( QPoint( X - 1, Y ));
		}

	} else {

		if( X < MAP_MAX_WIDTH - 1 )
		{
			if( Y > 0 )
			{
				l.append( QPoint( X + 1, Y - 1 ));
			}
			l.append( QPoint( X + 1, Y ));
		}
		if( Y < MAP_MAX_HEIGHT - 1 )
		{
			l.append( QPoint( X, Y + 1 ));
		}
		if( X > 0 )
		{
			l.append( QPoint( X - 1, Y ));
			if( Y > 0 )
			{
				l.append( QPoint( X - 1, Y - 1 ));
			}
		}

	}

	return l;
}

QList<QPoint> HexMap::GetAdjectentOwnedTiles(int X, int Y, int Owner)
{
	QList<QPoint> r = GetAdjectentTiles( X, Y );
	QList<QPoint> l;

	for(int i = 0; i < r.count(); i++ )
	{
		if( Tiles[r[i].x()][r[i].y()]->Owner == Owner )
		{
			l.append( QPoint(r[i].x(), r[i].y()) );
		}
	}

	return l;
}
