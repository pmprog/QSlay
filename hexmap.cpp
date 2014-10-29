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
	CurrentPlayer = -1;
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
					if( Tiles[x][y]->Owner == CurrentPlayer && Tiles[x][y]->HouseCash >= 10 )
					{
						painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(77, (SpriteFrame * 29) + 1, 18, 28) );
					} else {
						painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(96, 1, 18, 28) );
					}
				}

				if( Tiles[x][y]->HasMan )
				{
					int manx = (Tiles[x][y]->ManType * 19) + 1;
					if( Tiles[x][y]->Owner == CurrentPlayer && !Tiles[x][y]->ManMoved )
					{
						painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(manx, (SpriteFrame * 29) + 1, 18, 28) );
					} else {
						painter->drawPixmap( TileSpritePosition(x, y), *SpriteSheet, QRect(manx, 1, 18, 28) );
					}
				}

			}
		}
	}
}

void HexMap::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	GenerateMap(LastSeed + 1, 6, MAP_SIZE_SMALL);
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
	CurrentPlayer = 0;

	for( int x = 0; x < MAP_MAX_WIDTH; x++ )
	{
		for( int y = 0; y < MAP_MAX_HEIGHT; y++ )
		{
			Tiles[x][y]->Reset();
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

	// Populate houses
	for( int x = 0; x < MAP_MAX_WIDTH; x++ )
	{
		for( int y = 0; y < MAP_MAX_HEIGHT; y++ )
		{
			if( Tiles[x][y]->Owner >= 0 )
			{
				QList<QPoint> adjtlist;
				adjtlist.clear();
				adjtlist = GetAdjectentOwnedTiles( x, y, Tiles[x][y]->Owner );
				bool adjhashouse = false;
				for( int idx = 0; idx < adjtlist.size(); idx++ )
				{
					if( Tiles[adjtlist.at(idx).x()][adjtlist.at(idx).y()]->HasHouse )
					{
						adjhashouse = true;
						break;
					}
				}
				if( adjtlist.size() > 0 && adjhashouse == false )
				{
					Tiles[x][y]->HasHouse = true;
					Tiles[x][y]->HouseCash = 0;
				}
			}
		}
	}
	CombineHouses();

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

QList<QPoint> HexMap::GetGroupedTiles(int X, int Y)
{
	QList<QPoint> l;
	GetGroupedTilesRecurse( X, Y, &l );
	return l;
}

void HexMap::GetGroupedTilesRecurse(int X, int Y, QList<QPoint>* List)
{
	List->append(QPoint(X, Y));
	QList<QPoint> r = GetAdjectentOwnedTiles( X, Y, Tiles[X][Y]->Owner );
	for( int idx = 0; idx < r.size(); idx++ )
	{
		bool foundpoint = false;
		for( int fidx = 0; fidx < List->size(); fidx++ )
		{
			if( List->at(fidx).x() == r.at(idx).x() && List->at(fidx).y() == r.at(idx).y() )
			{
				foundpoint = true;
				break;
			}
		}

		if( !foundpoint )
		{
			GetGroupedTilesRecurse( r.at(idx).x(), r.at(idx).y(), List );
		}
	}
}

void HexMap::CombineHouses()
{
	QList<QPoint> processedlist;

	for( int x = 0; x < MAP_MAX_WIDTH; x++ )
	{
		for( int y = 0; y < MAP_MAX_HEIGHT; y++ )
		{
			bool alreadyprocessed = false;
			for( int idx = 0; idx < processedlist.size(); idx++ )
			{
				if( processedlist.at(idx).x() == x && processedlist.at(idx).y() == y )
				{
					alreadyprocessed = true;
					break;
				}
			}

			if( Tiles[x][y]->HasHouse && !alreadyprocessed )
			{
				QList<QPoint> grp = GetGroupedTiles( x, y );


				int groupcash = 0;
				for( int idx = 0; idx < grp.size(); idx++ )
				{
					if( Tiles[grp.at(idx).x()][grp.at(idx).y()]->HasHouse )
					{
						groupcash += Tiles[grp.at(idx).x()][grp.at(idx).y()]->HouseCash;
						Tiles[grp.at(idx).x()][grp.at(idx).y()]->HasHouse = false;
						Tiles[grp.at(idx).x()][grp.at(idx).y()]->HouseCash = 0;
					}
				}

				int randomtile = rand() % grp.size();
				Tiles[grp.at(randomtile).x()][grp.at(randomtile).y()]->HasHouse = true;
				Tiles[grp.at(randomtile).x()][grp.at(randomtile).y()]->HouseCash = groupcash;

				for( int idx = 0; idx < grp.size(); idx++ )
				{
					processedlist.append(QPoint(grp.at(idx).x(), grp.at(idx).y()));
				}

			}
		}
	}
}

void HexMap::StartNextTurn()
{
	QList<QPoint> processedlist;

	for( int x = 0; x < MAP_MAX_WIDTH; x++ )
	{
		for( int y = 0; y < MAP_MAX_HEIGHT; y++ )
		{
			bool alreadyprocessed = false;
			for( int idx = 0; idx < processedlist.size(); idx++ )
			{
				if( processedlist.at(idx).x() == x && processedlist.at(idx).y() == y )
				{
					alreadyprocessed = true;
					break;
				}
			}

			if( Tiles[x][y]->HasHouse && !alreadyprocessed )
			{
				QList<QPoint> grp = GetGroupedTiles( x, y );

				bool hasmen = false;
				for( int idx = 0; idx < grp.size(); idx++ )
				{
					HexTile* t = Tiles[grp.at(idx).x()][grp.at(idx).y()];

					// Graves turn to trees
					if( t->HasGrave )
					{
						t->HasGrave = false;
						t->HasTree = true;
						t->TreeType = 0;
					}
					// Trees spread
					if( t->HasTree )
					{
						// TODO: Spread trees
					} else {
						Tiles[x][y]->HouseCash++;
					}
					// Cost man
					if( t->HasMan )
					{
						Tiles[x][y]->HouseCash -= HexTile::ManCosts[t->ManType];
						hasmen = true;
					}
				}

				// No money? Kill men
				if( hasmen && Tiles[x][y]->HouseCash < 0 )
				{
					for( int idx = 0; idx < grp.size(); idx++ )
					{
						HexTile* t = Tiles[grp.at(idx).x()][grp.at(idx).y()];
						if( t->HasMan )
						{
							Tiles[x][y]->HasMan = false;
							Tiles[x][y]->HasGrave = true;
						}
					}
				}

				for( int idx = 0; idx < grp.size(); idx++ )
				{
					processedlist.append(QPoint(grp.at(idx).x(), grp.at(idx).y()));
				}

			}
		}
	}
}
