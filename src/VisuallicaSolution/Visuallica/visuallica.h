#ifndef VISUALLICA_H
#define VISUALLICA_H

#include <QtWidgets/QMainWindow>
#include "ui_Visuallica.h"
#include <QList>
#include <QPixmap>
#include "StateManager.h"

class QSplitter;
class QHBoxLayout;
class QVBoxLayout;
class QTabWidget;
class QListView;
class FileExplorer;
class ThumbnailView;
class GraphicsViewContainer;
class ThumbnailProgressBar;
class MultiViewSelectDialog;

class Visuallica : public QMainWindow
{
	Q_OBJECT

public:
	Visuallica(QWidget *parent = 0);
	~Visuallica();

public slots:
	void on_m_pActionPickTriggered();
	void on_m_pActionZoomTriggered();
	void on_m_pActionFitTriggered();
	void on_m_pActionOrgResolutionTriggered();
	void on_m_pActionPanTriggered();
	void on_m_pActionWindowLevelTriggered();

	void on_m_pActionReverseVerticalTriggered();
	void on_m_pActionReverseHorizontalTriggered();
	void on_m_pActionRotateLeftTriggered();
	void on_m_pActionRotateRightTriggered();
	void on_m_pActionRestoreMatrixTriggered();

	void on_m_pActionMultiViewTriggered();
	void on_m_pActionFullScreenTriggered();

	void on_m_pActionAboutTriggered();

	//void on_viewModeStateChanged(E_VIEWMODE);
	void on_behaviorModeStateChanged(E_BEHAVIORMODE);

private:

	Ui::VisuallicaClass ui;

	void initLayout();
	void initLayout2();
	void initActions();
	void initMenubar();
	void initToolbar();
	void initWidgets();
	void initQssStyle();

	QActionGroup * m_pActionGroupViewMode;
	QActionGroup * m_pActionGroupBehaviorMode;

	QAction* m_pActionPick;
	QAction* m_pActionZoom;
	QAction* m_pActionFit;
	QAction* m_pActionOrgResolution;
	QAction* m_pActionPan;
	QAction* m_pActionWindowLevel;

	QAction* m_pActionReverseVertical;
	QAction* m_pActionReverseHorizontal;
	QAction* m_pActionRotateLeft;
	QAction* m_pActionRotateRight;
	QAction* m_pActionRestoreMatrix;

	QAction* m_pActionMultiView;
	QAction* m_pActionFullScreen;

	QAction* m_pActionAbout;

	QAction* m_pActionTestDcmLoad;

	QMenu* m_pMenuView;
	QMenu* m_pMenuTools;
	QMenu* m_pMenuHelp;

	QSplitter * m_pRightSplitter;
	ThumbnailView * m_pThumbnailView;

	ThumbnailProgressBar * m_pThumbnailProgressBar;

	QTabWidget * m_pTabMain;

	GraphicsViewContainer * m_pGraphicsViewContainer;
	QSplitter * m_pMainSplitter;

	QTabWidget * m_pTabLeft;
	QWidget * m_pFileExplorerTabInTabLeft;
	QVBoxLayout * m_pVBoxLayoutInFileExplorerTabInTabLeft;
	FileExplorer *m_pFileExplorer;


	MultiViewSelectDialog *m_pMultiViewSelectionDlg;

	bool m_bisMaximizedBeforeFullScreen;
	// 테스트용 
	//QList<QPixmap> m_thumbnailPixmapList;


};

#endif // VISUALLICA_H
