#include "BGraphicsScene.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QGraphicsPixmapItem>
#include <QDebug>

BGraphicsScene::BGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
{
	imageItem = NULL;


}

void BGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	

	if (event->mimeData()->hasFormat("image/x-thumbnail"))
	{
		event->setDropAction(Qt::CopyAction);
		event->acceptProposedAction();
		//event->accept();
		qDebug() << "BGraphicsScene::dragEnterEvent invoked!! accept!";
	}
	else
	{
		event->ignore();
		qDebug() << "BGraphicsScene::dragEnterEvent invoked!! ignore!";
	}
	
	//return QGraphicsScene::dragEnterEvent(event);
}

void BGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	
	if (event->mimeData()->hasFormat("image/x-thumbnail"))
	{
		event->setDropAction(Qt::CopyAction);
		//event->acceptProposedAction();
		event->accept();
		qDebug() << "BGraphicsScene::dragMoveEvent invoked!! accept!";
	}
	else
	{
		event->ignore();
		qDebug() << "BGraphicsScene::dragMoveEvent invoked!! ignore!";
	}

	//return QGraphicsScene::dragMoveEvent(event);
}

