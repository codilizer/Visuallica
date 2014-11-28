#ifndef GRAPHICSVIEWCONTAINER_H
#define GRAPHICSVIEWCONTAINER_H

#include <QWidget>
#include <QList>
#include "Observer.h"


class QHBoxLayout;
class BGraphicsView;
class QGridLayout;
class MultiViewManager;


class GraphicsViewContainer : public QWidget, public Observer
{
	Q_OBJECT

public:
	GraphicsViewContainer(MultiViewManager* pMultiViewManager, QWidget *parent = 0);
	~GraphicsViewContainer();

	virtual void Update(Subject * theChangeSubject);

private:
	void initLayout();
	void changeMultiView(int rowCount, int colCount);

	MultiViewManager *m_pMultiViewManager;
	QGridLayout *m_pGridLayout;
	BGraphicsView **m_pGraphicsView;

	int m_iCountGraphicsView;
	int m_iRowCount;
	int m_iColumnCount;

};


#endif // GRAPHICSVIEWCONTAINER_H