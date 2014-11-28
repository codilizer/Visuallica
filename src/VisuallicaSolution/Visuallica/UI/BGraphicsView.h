#ifndef BGRAPHICSVIEW_H
#define BGRAPHICSVIEW_H

#include <QGraphicsView>
#include "Observer.h"
#include "StateManager.h"
#include "BPixmap.h"
#include <QSCroller>
class QDropEvent;
class QDragEnterEvent;
class QLabel;
class BGraphicsScene;
class ImageViewThread;
class QGraphicsPixmapItem;
class StateManager;
class QImage;
class QMutex;


class BGraphicsView : public QGraphicsView, public Observer
{
	Q_OBJECT

public:
	BGraphicsView(StateManager* subject, QWidget * parent = 0);
	~BGraphicsView();


public slots:
	void LoadImageResultByThread(BPixmap* image, QString path);

	void ProcessScale(float ratio);
	void ProcessWindow(QPoint, QPoint);


	virtual void Update(Subject * theChangeSubject);

	void ScrollerStateChanged(QScroller::State newState);
	void HScrollBarRangeChanged(int, int);
	void VScrollBarRangeChanged(int, int);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dragMoveEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent * event);

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	virtual void drawForeground(QPainter *painter, const QRectF &rect);

private:
	enum E_VIEWMODE { FITWINDOW_VIEW, ORG_RESOLUTION_VIEW, NORMAL_VIEW };
	void changeViewMode(E_VIEWMODE);
	void changeBehaviorMode(E_BEHAVIORMODE);
	void display();
	void setSceneAndBackground(QGraphicsScene*, Qt::GlobalColor);
	qreal getCurrentZoomFactor();
	void updateImage(QImage* newImage);

	StateManager * m_pStateManager;

	BGraphicsScene * m_pImageScene;
	QGraphicsPixmapItem *m_pPixmapItem;
	BPixmap *m_pCurBPixmap;

	ImageViewThread *m_pImageViewThread;

	
	E_VIEWMODE m_eViewMode;
	

	E_BEHAVIORMODE m_eBehaviorMode;


	int m_iHScrollbarPos;
	int m_iVScrollbarPos;
	int m_iHScrollbarMaxRange;
	int m_iVScrollbarMaxRange;

	QPointF m_ViewCenterOfSceneBeforeZooming;

	int m_iZoomStartMousePointY;
	QPoint m_fZoomPrevMousePoint;
	int m_iPrevMousePointY;
	bool m_bZooming;
	float m_fScaleFactor;
	QPoint zoomEnteredMouseCursorPoint;

	QPoint m_fWindowPrevMousePoint;
	double m_dPrevWindowWidth;
	double m_dPrevWindowCenter;
	bool m_bWindowing;

	QMutex *m_pMutex;
};




#endif // BGRAPHICSVIEW_H