#ifndef GAME_H
#define GAME_H

#include "qtlibs.h"
#include "hexmap.h"

class Game : public QObject
{
    Q_OBJECT

	public:
			explicit Game(QObject *parent = 0);

	signals:

	public slots:
		void Animate();
};

#endif // GAME_H
