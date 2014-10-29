#include "main.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	appObj = &app;

	QGraphicsScene scene;
	sceneObj = &scene;
	scene.setItemIndexMethod(QGraphicsScene::NoIndex);

	Map = new HexMap( 23 );
	scene.addItem( Map );
	Map->setPos( 10, 20 );

	CurrentGame = new Game();
	Map->GenerateMap( 0, 6, MAP_SIZE_SMALL );

	//scene.setSceneRect( 0, 0, Map->boundingRect().width() + 20, Map->boundingRect().height() + 30);

	QGraphicsView view( &scene );
	viewObj = &view;
	view.setRenderHint(QPainter::Antialiasing);
	view.setBackgroundBrush( QColor( 128, 140, 164 ) );
	view.setCacheMode(QGraphicsView::CacheBackground);
	view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "QSlay"));
	view.setAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
	view.show();
	view.setGeometry( 80, 80, 790, 400 );


	QTimer timer;
	QObject::connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
	timer.start(250);

	QTimer timer2;
	QObject::connect(&timer2, SIGNAL(timeout()), CurrentGame, SLOT(Animate()));
	timer2.start(250);

	return app.exec();
}

