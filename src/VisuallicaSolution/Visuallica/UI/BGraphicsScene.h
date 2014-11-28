#ifndef BGRAPHICSSCENE_H
#define BGRAPHICSSCENE_H


#include <QGraphicsScene>

class QGraphicsSceneDragDropEvent;
class QGraphicsPixmapItem;


class BGraphicsScene : public QGraphicsScene
{
public:
	BGraphicsScene(QObject *parent = 0);

protected:
	virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

private:
	QGraphicsPixmapItem *imageItem;

};

#endif //BGRAPHICSSCENE_H