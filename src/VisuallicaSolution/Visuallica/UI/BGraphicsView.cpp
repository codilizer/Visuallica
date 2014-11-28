#include "BGraphicsView.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QLabel>
//#include <QGraphicsScene>
#include "BGraphicsScene.h"
#include <QGraphicsPixmapItem>
//#include <QAbstractScrollArea>
#include <QScrollBar>
#include <QDebug>
#include "ImageViewThread.h"
#include "DcmPixmap.h"
//#include "BDcm.h"
#include <QPainter>
#include "ResourceDefines.h"
#include <QMutex>
#include <QMatrix>

BGraphicsView::BGraphicsView(StateManager *pStateManager, QWidget *parent)
	:QGraphicsView(parent)
{
	setAcceptDrops(true);

	m_pImageScene = NULL;
	m_pPixmapItem = NULL;
	m_pCurBPixmap = NULL;

	m_fScaleFactor = 1;

	m_eViewMode = NORMAL_VIEW;

	m_pStateManager = pStateManager;
	m_pStateManager->Attach(this);

	m_pImageViewThread = new ImageViewThread();

	connect(m_pImageViewThread, &ImageViewThread::ImageViewThreadResultLoadImage,
		this, &BGraphicsView::LoadImageResultByThread);

	m_pImageViewThread->start();

	QScroller *pScroller = QScroller::scroller(this);
	connect(pScroller, &QScroller::stateChanged,
		this, &BGraphicsView::ScrollerStateChanged);


	m_pMutex = new QMutex();
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate); // Foregroud Update

}

BGraphicsView::~BGraphicsView()
{ 
	m_pStateManager->Detach(this);

	if (m_pPixmapItem)
		delete m_pPixmapItem;

	if (m_pCurBPixmap)
		delete m_pCurBPixmap;

	if (m_pImageScene)
		delete m_pImageScene;

}


void BGraphicsView::Update(Subject * paChangeSubject)
{
	qDebug() << "BGraphicsView::Update() Invoked !!";

	if (paChangeSubject == m_pStateManager)
	{
		E_BEHAVIORMODE newBehaviorMode = m_pStateManager->GetBehaviorMode();
		if (m_eBehaviorMode != newBehaviorMode)
		{
			changeBehaviorMode(newBehaviorMode);
		}

	}
}

void BGraphicsView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event); // 이거 안넣으면 사이즈 변경시 센터에 출력 안되고 좌표 바뀜 
	if (FITWINDOW_VIEW == m_eViewMode)
		display();
}

void BGraphicsView::changeViewMode(E_VIEWMODE eaViewMode)
{
	if (m_eViewMode != eaViewMode)
	{
		m_eViewMode = eaViewMode;
		display();
	}
}

void BGraphicsView::changeBehaviorMode(E_BEHAVIORMODE eaBehaviorMode)
{
	QScroller::ungrabGesture(this);

	int behaviorIconSize = 32;

	if (E_BEHAVIORMODE::PICK == eaBehaviorMode)
	{		
		viewport()->setCursor(Qt::ArrowCursor);
	}
	else if (E_BEHAVIORMODE::FITWINDOW == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			viewport()->setCursor(QCursor(QPixmap(DEF_FIT_IN_VIEW_ICON_PATH).scaled(QSize(behaviorIconSize, behaviorIconSize))));
		}	
	}
	else if (E_BEHAVIORMODE::ORG_RESOLUTION == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			viewport()->setCursor(QCursor(QPixmap(DEF_ORG_RESOLUTION_ICON_PATH).scaled(QSize(behaviorIconSize, behaviorIconSize))));
		}
	}
	else if (PAN == eaBehaviorMode)
	{		
		QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
		viewport()->setCursor(Qt::OpenHandCursor);
	}
	else if (E_BEHAVIORMODE::ZOOM == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			viewport()->setCursor(Qt::SizeVerCursor);
		}
	}
	else if (E_BEHAVIORMODE::WINDOW == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			if (m_pCurBPixmap->isDcmGrayImage())
				viewport()->setCursor(Qt::SizeAllCursor);
			else
				viewport()->setCursor(Qt::ArrowCursor);
		}
		
	}
	else if (E_BEHAVIORMODE::REVERSE_VERTICAL == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			viewport()->setCursor(QCursor(QPixmap(DEF_REVERSE_VERTICAL_ICON_PATH).scaled(QSize(behaviorIconSize, behaviorIconSize))));
		}
	}
	else if (E_BEHAVIORMODE::REVERSE_HORIZONTAL == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			viewport()->setCursor(QCursor(QPixmap(DEF_REVERSE_HORIZONTAL_ICON_PATH).scaled(QSize(behaviorIconSize, behaviorIconSize))));
		}
	}
	else if (E_BEHAVIORMODE::ROTATE_LEFT == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			viewport()->setCursor(QCursor(QPixmap(DEF_ROTATE_LEFT_ICON_PATH).scaled(QSize(behaviorIconSize, behaviorIconSize))));
		}
	}
	else if (E_BEHAVIORMODE::ROTATE_RIGHT == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			viewport()->setCursor(QCursor(QPixmap(DEF_ROTATE_RIGHT_ICON_PATH).scaled(QSize(behaviorIconSize, behaviorIconSize))));
		}
	}
	else if (E_BEHAVIORMODE::RESTORE_MATRIX == eaBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			viewport()->setCursor(QCursor(QPixmap(DEF_RESTORE_MATRIX_ICON_PATH).scaled(QSize(behaviorIconSize, behaviorIconSize))));
		}
	}

	m_eBehaviorMode = eaBehaviorMode;	
}


void BGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{	
	QGraphicsView::dragEnterEvent(event);

	if (event->mimeData()->hasFormat("image/x-thumbnail"))
	{
		event->setDropAction(Qt::CopyAction);
		event->accept();
		//event->accept();
		qDebug() << "BGraphicsView::dragEnterEvent invoked!! accept!";
	}
	else
	{
		event->ignore();
		qDebug() << "BGraphicsView::dragEnterEvent invoked!! ignore!";
	}

	//return QGraphicsView::dragEnterEvent(event);
}

void BGraphicsView::dragMoveEvent(QDragEnterEvent *event)
{
	QGraphicsView::dragMoveEvent(event);

	if (event->mimeData()->hasFormat("image/x-thumbnail"))
	{
		event->setDropAction(Qt::CopyAction);
		event->acceptProposedAction();
		//event->accept();
		qDebug() << "BGraphicsView::dragMoveEvent invoked!! accept!";
	}
	else
	{
		event->ignore();
		qDebug() << "BGraphicsView::dragMoveEvent invoked!! ignore!";
	}
	
	//QGraphicsView::dragMoveEvent(event);
	//return QGraphicsView::dragMoveEvent(event);
}
//
void BGraphicsView::dropEvent(QDropEvent * event)
{
	QGraphicsView::dropEvent(event);

	qDebug() << "BGraphicsView::dropEvent invoked!!";
	if (event->mimeData()->hasFormat("image/x-thumbnail"))
	{
		qDebug() << "BGraphicsView::dropEvent invoked!! accept!";
		QByteArray pieceData = event->mimeData()->data("image/x-thumbnail");
		QDataStream stream(&pieceData, QIODevice::ReadOnly);
		QString filePath;
		stream >> filePath;


		if (filePath.isNull()) {
			qDebug("*** Error - Null filePath");
		}
		else {
			m_pImageViewThread->executeThreadLoadImage(filePath, m_pMutex);
		}

		event->setDropAction( Qt::CopyAction );
		event->acceptProposedAction(); // 없으면 최초 1회만 드랍됨
	}

	//QGraphicsView::dropEvent(event);
	//return QGraphicsView::dropEvent(event);
}

void BGraphicsView::mousePressEvent(QMouseEvent *event)
{
	if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::FITWINDOW == m_eBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			changeViewMode(FITWINDOW_VIEW);
		}
	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::ORG_RESOLUTION == m_eBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			changeViewMode(ORG_RESOLUTION_VIEW);
		}
	}
	else if(Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::ZOOM == m_eBehaviorMode)
	{

		m_fZoomPrevMousePoint = event->pos();

		m_iZoomStartMousePointY = m_fZoomPrevMousePoint.y();
		m_iPrevMousePointY = m_fZoomPrevMousePoint.y();

		if (!m_bZooming){
			//setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		}
		
		m_iHScrollbarPos = horizontalScrollBar()->sliderPosition();
		m_iVScrollbarPos = verticalScrollBar()->sliderPosition();

		QPoint viewCenter = QPoint(width() / 2, height() / 2);
		m_ViewCenterOfSceneBeforeZooming =  mapToScene(viewCenter);

		m_bZooming = true;
	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::WINDOW == m_eBehaviorMode)
	{
		m_fWindowPrevMousePoint = event->pos();
		m_bWindowing = true;

	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::REVERSE_VERTICAL == m_eBehaviorMode)
	{
		if (m_pCurBPixmap)
		{

			//scale(-1, 1);
			m_pCurBPixmap->Scale(-1, 1);
			updateImage(m_pCurBPixmap->GetTransformedImage());
		}
	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::REVERSE_HORIZONTAL == m_eBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			//scale(1, -1);
			m_pCurBPixmap->Scale(1, -1);
			updateImage(m_pCurBPixmap->GetTransformedImage());
		}
	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::ROTATE_LEFT == m_eBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			//rotate(-90);
			m_pCurBPixmap->Rotate(-90);
			updateImage(m_pCurBPixmap->GetTransformedImage());
		}
	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::ROTATE_RIGHT == m_eBehaviorMode)
	{
		qDebug() << "E_BEHAVIORMODE::ROTATE_RIGHT == m_eBehaviorMode";
		if (m_pCurBPixmap)
		{
			//QTransform matrix;

			//matrix.rotate(90);
			//setTransform(matrix);

			
			//rotate(90);
			m_pCurBPixmap->Rotate(90);
			updateImage(m_pCurBPixmap->GetTransformedImage());
		}
	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::RESTORE_MATRIX == m_eBehaviorMode)
	{
		if (m_pCurBPixmap)
		{
			m_pCurBPixmap->ResetTransform();
			updateImage(m_pCurBPixmap->GetTransformedImage());
		}
	}

	return QGraphicsView::mousePressEvent(event);
}

void BGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

	if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::ZOOM == m_eBehaviorMode && m_bZooming)
	{
		m_fZoomPrevMousePoint = event->pos();
		int newMousePointY = event->pos().ry();

		int posGap = m_iPrevMousePointY - newMousePointY;
		
		m_fScaleFactor = 1 + posGap * 0.005;
		scale(m_fScaleFactor, m_fScaleFactor);
		m_iPrevMousePointY = newMousePointY;

		changeViewMode(NORMAL_VIEW);
	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::WINDOW == m_eBehaviorMode && m_bWindowing)
	{
		QPoint newMousePoint = event->pos();

		ProcessWindow(m_fWindowPrevMousePoint, newMousePoint);

		m_fWindowPrevMousePoint = newMousePoint;
	}




	return QGraphicsView::mouseMoveEvent(event);
}

void BGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{

	if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::ZOOM == m_eBehaviorMode)
	{
		int newMousePointY = event->pos().ry();
		m_bZooming = false;
	}
	else if (Qt::MouseEventNotSynthesized == event->source()
		&& E_BEHAVIORMODE::WINDOW == m_eBehaviorMode)
	{
		m_bWindowing = false;

	}

	return QGraphicsView::mouseReleaseEvent(event);
}

void BGraphicsView::ScrollerStateChanged(QScroller::State newState)
{
	qDebug() << "ScrollerStateChanged : " << newState;
	//1, 3
	if (m_eBehaviorMode == PAN)
	{
		if (QScroller::State::Pressed == newState)
		{
			viewport()->setCursor(Qt::ClosedHandCursor);
		}
		else if (QScroller::State::Inactive == newState)
		{
			viewport()->setCursor(Qt::OpenHandCursor);
		}
	}
}


void BGraphicsView::HScrollBarRangeChanged(int min, int max)
{
	if (0 == max)
		return;

	if (E_BEHAVIORMODE::ZOOM == m_eBehaviorMode && m_bZooming)
	{
		int offset = 0;
		offset = max - m_iHScrollbarMaxRange;

		m_iHScrollbarPos = m_iHScrollbarPos - offset;
		horizontalScrollBar()->setSliderPosition(m_iHScrollbarPos);
	}

	m_iHScrollbarMaxRange = max;
}

void BGraphicsView::VScrollBarRangeChanged(int min, int max)
{
	if (0 == max)
		return;

	if (E_BEHAVIORMODE::ZOOM == m_eBehaviorMode && m_bZooming)
	{
		int offset = 0;
		offset = max - m_iVScrollbarMaxRange;

		m_iVScrollbarPos = m_iVScrollbarPos - offset;
		verticalScrollBar()->setSliderPosition(m_iVScrollbarPos);
	}


	m_iVScrollbarMaxRange = max;
}

void BGraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
	invalidateScene();
	painter->resetTransform();

	if (m_pCurBPixmap)
	{
		if (m_pCurBPixmap->isDcmGrayImage())
		{
			qDebug() << "BGraphicsView::drawForeground() invoked!!";

			DcmPixmap *pDcmPixmap = static_cast<DcmPixmap*>(m_pCurBPixmap);
			painter->setPen(Qt::white);
			painter->setFont(QFont("Helvetica", 11, QFont::Normal));

			
			//QPoint topLeft = this->rect().topLeft();
			QPoint topLeft = viewport()->rect().topLeft();
			
			QString strPatientName = pDcmPixmap->GetPatientName();
			QString strPatientId = pDcmPixmap->GetPatientID();
			QString strPatientBirthdate = pDcmPixmap->GetPatientBirthDate();
			QString strModality = pDcmPixmap->GetModality();

			QString strTopLeft;
			
			if (!strPatientName.isEmpty())
				strTopLeft += strPatientName + "\n";
			if (!strPatientId.isEmpty())
				strTopLeft += strPatientId + "\n";
			if (!strPatientBirthdate.isEmpty())
				strTopLeft += strPatientBirthdate + "\n";
			if (!strModality.isEmpty())
			    strTopLeft != strModality;

			//QPoint topRight = this->rect().topRight();
			QPoint topRight = viewport()->rect().topRight();

			QString strInstitutionName = pDcmPixmap->GetInstitutionName();
			QString strRefferingPhysicianName = pDcmPixmap->GetRefferingPhysicianName();
			QString strStudyDate = pDcmPixmap->GetStudyDate();
			QString strStudyTime = pDcmPixmap->GetStudyTime();

			QString strTopRight;
			if (!strInstitutionName.isEmpty())
				strTopRight += strInstitutionName + "\n";
			if (!strRefferingPhysicianName.isEmpty())
				strTopRight += strRefferingPhysicianName + "\n";
			if (!strStudyDate.isEmpty())
				strTopRight += strStudyDate + "\n";
			if (!strStudyTime.isEmpty())
				strTopRight += strStudyTime;

			//QPoint bottomLeft = this->rect().bottomLeft();
			QPoint bottomLeft = viewport()->rect().bottomLeft();

			QString strSeriesDescription = pDcmPixmap->GetSeriesDescription();
			QString strBottomLeft = strSeriesDescription;


			//QPoint bottomRight = this->rect().bottomRight();
			QPoint bottomRight = viewport()->rect().bottomRight();

			// sprintf 안에서 tr()쓰면 출력 제대로 안되었음
			QString strWindowCenter;
			strWindowCenter = strWindowCenter.sprintf("%s%-6.2f", "L : ", m_dPrevWindowCenter);
			QString strWindowWidth;
			strWindowWidth.sprintf("%s%-6.2f", "W : ", m_dPrevWindowWidth);
			QString strZoom;
			if (ORG_RESOLUTION == m_eViewMode)
				strZoom.sprintf("%s\t%d%", "Zoom :", 100);
			else
				strZoom.sprintf("%s\t%d%", "Zoom :", qRound64(getCurrentZoomFactor() * 100));
				//strZoom.sprintf("%s\t%d%", "Zoom :", qRound64(qAbs(matrix().m11()) * 100));

			QString strBottomRight = strWindowCenter + "\n" + strWindowWidth + "\n" + strZoom;

			painter->drawText(topLeft.x() + 16, topLeft.y() + 16, 184, 184, Qt::AlignLeft | Qt::AlignTop, strTopLeft);
			painter->drawText(topRight.x() - 200, topRight.y() + 16, 184, 184, Qt::AlignRight | Qt::AlignTop, strTopRight);
			painter->drawText(bottomLeft.x() + 16, bottomLeft.y() - 200, 184, 184, Qt::AlignLeft | Qt::AlignBottom, strBottomLeft);
			painter->drawText(bottomRight.x() - 200, bottomRight.y() - 200, 184, 184, Qt::AlignRight | Qt::AlignBottom, strBottomRight);

		}
	}
}

void BGraphicsView::LoadImageResultByThread(BPixmap* argPixmapimage, QString path)
{
	if (m_pImageScene) {
		if (m_pPixmapItem) {
			m_pImageScene->removeItem(m_pPixmapItem);
			delete m_pPixmapItem;
			m_pPixmapItem = NULL;
		}

		delete m_pImageScene;
		m_pImageScene = NULL;
	}

	if (m_pCurBPixmap)
	{
		delete m_pCurBPixmap;
		m_pCurBPixmap = NULL;
	}
	m_pCurBPixmap = argPixmapimage;

	if (!m_pCurBPixmap->isDcmGrayImage())
	{
		QImage *pImage = new QImage(argPixmapimage->toImage());
		m_pCurBPixmap->SetOriginalImage(pImage); 
	}


	m_pImageScene = new BGraphicsScene();
	m_pPixmapItem = m_pImageScene->addPixmap(*argPixmapimage);


	if (m_pCurBPixmap->isDcmGrayImage())
	{
		static_cast<DcmPixmap*>(m_pCurBPixmap)->GetWindowValue(m_dPrevWindowCenter, m_dPrevWindowWidth);
	}



	setSceneAndBackground(m_pImageScene, Qt::black);

	QMatrix mat = matrix();
	mat.reset();
	setMatrix(mat);

	if (viewport()->rect().width() < m_pCurBPixmap->rect().width() ||
		viewport()->rect().height() < m_pCurBPixmap->rect().height())
	{
		changeViewMode(FITWINDOW_VIEW);
	}
	else
	{
		changeViewMode(ORG_RESOLUTION_VIEW);
	}

	StateManager::GetInstance().SetBehaviorMode(PICK);

	display();
}

void BGraphicsView::ProcessScale(float ratio)
{
	qDebug() << "ProcessScale ratio : " << ratio;
	
	QImage *pImage = m_pCurBPixmap->GetTransformedImage();

	int scaleWidth = pImage->rect().width() * ratio;
	int scaleHeight = pImage->rect().height() *ratio;
	
	QImage pScaledImage = pImage->scaled(scaleWidth, scaleHeight);

	if (pImage)
	{
		delete pImage;
		pImage = NULL;
	}

	QPixmap pixmap;

	if (m_pCurBPixmap->isDcmGrayImage())
		pixmap.convertFromImage(pScaledImage, Qt::NoFormatConversion | Qt::MonoOnly);
	else
		pixmap.convertFromImage(pScaledImage, Qt::NoFormatConversion);


	m_pPixmapItem->setPixmap(pixmap);
}



void BGraphicsView::ProcessWindow(QPoint prevMousePoint, QPoint newMousePoint)
{
	if (!m_pCurBPixmap->isDcmGrayImage())
		return;

	int gap_y = newMousePoint.ry() - prevMousePoint.ry() ;
	int gap_x = newMousePoint.rx() - prevMousePoint.rx();
		
	double prevCenter, prevWidth;
	
	static_cast<DcmPixmap*>(m_pCurBPixmap)->GetWindowValue(prevCenter, prevWidth);


	int windowCenter = prevCenter - gap_x;
	int windowWidth = prevWidth - gap_y;

	m_dPrevWindowCenter = windowCenter;
	m_dPrevWindowWidth = windowWidth;

	QImage * windowedImage = static_cast<DcmPixmap*>(m_pCurBPixmap)->SetWindow(windowCenter, windowWidth);

	QPixmap pixmap;

	if (m_pCurBPixmap->isDcmGrayImage())
		pixmap.convertFromImage(*windowedImage, Qt::NoFormatConversion | Qt::MonoOnly);
	else
		pixmap.convertFromImage(*windowedImage, Qt::NoFormatConversion);

	if (windowedImage) {
		delete windowedImage;
		windowedImage = NULL;
	}

	setSceneAndBackground(m_pImageScene, Qt::black);

	m_pPixmapItem->setPixmap(pixmap);

	if (m_pImageScene)
		m_pImageScene->invalidate(); // for redraw foreground
}

void BGraphicsView::updateImage(QImage* newImage)
{
	QPixmap pixmap;

	if (m_pCurBPixmap->isDcmGrayImage())
		pixmap.convertFromImage(*newImage, Qt::NoFormatConversion | Qt::MonoOnly);
	else
		pixmap.convertFromImage(*newImage, Qt::NoFormatConversion);

	if (m_pImageScene) {
		if (m_pPixmapItem) {
			m_pImageScene->removeItem(m_pPixmapItem);
			delete m_pPixmapItem;
			m_pPixmapItem = NULL;
		}

		delete m_pImageScene;
		m_pImageScene = NULL;
	}

	m_pImageScene = new BGraphicsScene();
	m_pPixmapItem = m_pImageScene->addPixmap(pixmap);

	setSceneAndBackground(m_pImageScene, Qt::black);

	if (FITWINDOW == m_eViewMode)
	{
		fitInView(m_pPixmapItem, Qt::KeepAspectRatio);
	}
	else if (ORG_RESOLUTION == m_eViewMode)
	{

		fitInView(rect(), Qt::KeepAspectRatio);
	}
	else
	{
		scale(m_fScaleFactor, m_fScaleFactor);
	}

}

void BGraphicsView::setSceneAndBackground(QGraphicsScene* paGraphicsScene, Qt::GlobalColor aColor)
{
	setScene(paGraphicsScene);
	setBackgroundBrush(QBrush(aColor, Qt::SolidPattern));
}


void BGraphicsView::display()
{
	if (m_pImageScene)
	{
		QRectF scenRect = m_pImageScene->sceneRect();
		int sceneWidth = m_pImageScene->width();
		int sceneheight = m_pImageScene->height();
		qDebug() << "Scene rect : " << sceneRect().left() << ", " << sceneRect().right() << ", " << sceneRect().width() << ", " << sceneRect().height();
		qDebug() << "Scene Width : " << sceneWidth;
	}

	if (NULL == m_pPixmapItem)
		return;

	qDebug() << "BGraphicsView::display() invoked!";

	if (FITWINDOW_VIEW == m_eViewMode)
	{
		//Ensures that item fits tightly inside the view, scaling the view
		fitInView(m_pPixmapItem, Qt::KeepAspectRatio);
	}
	else if (ORG_RESOLUTION_VIEW == m_eViewMode)
	{
		fitInView(rect(), Qt::KeepAspectRatio);
		centerOn(m_pPixmapItem);
	}
}

qreal BGraphicsView::getCurrentZoomFactor()
{
	qreal rMax = 0;
	
	if (rMax < qAbs(matrix().m11()))
		rMax = qAbs(matrix().m11());

	if (rMax < qAbs(matrix().m12()))
		rMax = qAbs(matrix().m12());

	if (rMax < qAbs(matrix().m21()))
		rMax = qAbs(matrix().m21());

	if (rMax < qAbs(matrix().m22()))
		rMax = qAbs(matrix().m22());

	return rMax;
}
