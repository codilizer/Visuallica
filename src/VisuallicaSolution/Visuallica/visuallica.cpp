#include "Visuallica.h"
#include "QSplitter.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "QTabWidget.h"
//#include "QListView.h"
#include "ThumbnailView.h"
#include "FileExplorer.h"
#include "GraphicsViewContainer.h"
#include <QDebug>
#include <QFile>
#include "ThumbnailProgressBar.h"
#include "ThumbnailProgressManager.h"
#include "ResourceDefines.h"
#include "MultiViewSelectDialog.h"
#include "MultiViewManager.h"
#include <QMessageBox>

Visuallica::Visuallica(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	initQssStyle();
	
	initActions();
	initMenubar();
	initToolbar();
	initLayout();
	
	initWidgets();
	
}

Visuallica::~Visuallica()
{

}

void Visuallica::initQssStyle()
{
	//QFile styles(":/Qss/Resources/qss/darkViolet.stylesheet");
	QFile styles(":/Qss/Resources/qss/darkorange.stylesheet");
	//QFile styles(":/Qss/Resources/qss/darkstyle.qss");
	//QFile styles(":/Qss/Resources/qss/QTDark.stylesheet");

	if (styles.open(QIODevice::ReadOnly)) {
		setStyleSheet(styles.readAll());
	}

	// always on top
	//setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	//raise();

	activateWindow();

	//setWindowOpacity(0.5);

}

void Visuallica::initLayout()
{
	setWindowIcon(QIcon(":/Icons/Resources/Icons/Main.ico"));
	// 테스트용
	m_pFileExplorer = new FileExplorer(this);
	m_pThumbnailView = new ThumbnailView(m_pFileExplorer, this);

	m_pThumbnailProgressBar = new ThumbnailProgressBar(&ThumbnailProgressManager::GetInstance());
	m_pThumbnailProgressBar->setVisible(false);

	m_pRightSplitter = new QSplitter(Qt::Vertical, this);
	m_pTabMain = new QTabWidget(this);
	m_pRightSplitter->addWidget(m_pThumbnailView);
	m_pRightSplitter->addWidget(m_pThumbnailProgressBar);
	m_pRightSplitter->addWidget(m_pTabMain);
	m_pRightSplitter->setStretchFactor(2, 1);

	m_pGraphicsViewContainer = new GraphicsViewContainer(&MultiViewManager::GetInstance(), this);

	m_pTabMain->addTab(m_pGraphicsViewContainer, tr("Viewer"));

	m_pTabLeft = new QTabWidget(this);

	m_pMainSplitter = new QSplitter(Qt::Horizontal, this);

	m_pMainSplitter->addWidget(m_pTabLeft);
	m_pMainSplitter->addWidget(m_pRightSplitter);

	m_pMainSplitter->setStretchFactor(1, 1);

	setCentralWidget(m_pMainSplitter);

	m_pFileExplorerTabInTabLeft = new QWidget(this);


	//m_pFileExplorer->setAutoScroll(true);	

	m_pVBoxLayoutInFileExplorerTabInTabLeft = new QVBoxLayout(this);

	m_pFileExplorerTabInTabLeft->setLayout(m_pVBoxLayoutInFileExplorerTabInTabLeft);
	m_pVBoxLayoutInFileExplorerTabInTabLeft->addWidget(m_pFileExplorer);

	m_pTabLeft->addTab(m_pFileExplorerTabInTabLeft, tr("Explorer"));

}

void Visuallica::initActions()
{
	m_pActionOrgResolution = new QAction(tr("&1:1 Resolution"), this);
	m_pActionOrgResolution->setIcon(QIcon(DEF_ORG_RESOLUTION_ICON_PATH));
	//m_pActionOrgResolution->setText(tr("1:1 Resolution"));	
	m_pActionOrgResolution->setCheckable(true);
	connect(m_pActionOrgResolution, &QAction::triggered,
		this, &Visuallica::on_m_pActionOrgResolutionTriggered);

	m_pActionZoom = new QAction(tr("Zoom In / Out"), this);
	m_pActionZoom->setIcon(QIcon(":/Icons/Resources/Icons/IconZoom_128.png"));
	//m_pActionZoom->setText(tr("&Zoom In / Out"));	
	m_pActionZoom->setCheckable(true);
	connect(m_pActionZoom, &QAction::triggered,
		this, &Visuallica::on_m_pActionZoomTriggered);

	m_pActionFit = new QAction(tr("Fit to window"), this);
	m_pActionFit->setIcon(QIcon(DEF_FIT_IN_VIEW_ICON_PATH));
	//m_pActionFit->setText(tr("&Fit to window"));	
	m_pActionFit->setCheckable(true);
	connect(m_pActionFit, &QAction::triggered,
		this, &Visuallica::on_m_pActionFitTriggered);

	m_pActionPan = new QAction(tr("&Pan"), this);
	m_pActionPan->setIcon(QIcon(":/Icons/Resources/Icons/IconHandGrab_128.png"));
	//m_pActionPan->setText(tr("Pan"));
	m_pActionPan->setCheckable(true);
	connect(m_pActionPan, &QAction::triggered,
		this, &Visuallica::on_m_pActionPanTriggered);

	m_pActionPick = new QAction(tr("&Pick tool"),this);
	m_pActionPick->setIcon(QIcon(":/Icons/Resources/Icons/IconiArrow_128.png"));
	//m_pActionPick->setText(tr("Pick tool"));	
	m_pActionPick->setCheckable(true);
	connect(m_pActionPick, &QAction::triggered,
		this, &Visuallica::on_m_pActionPickTriggered);

	m_pActionWindowLevel = new QAction(tr("&Window Level"), this);
	m_pActionWindowLevel->setIcon(QIcon(":/Icons/Resources/Icons/WindowLevel_128.png"));
	m_pActionWindowLevel->setCheckable(true);
	connect(m_pActionWindowLevel, &QAction::triggered,
		this, &Visuallica::on_m_pActionWindowLevelTriggered);

	m_pActionReverseVertical = new QAction(tr("&Reverse Vertical"), this);
	m_pActionReverseVertical->setIcon(QIcon(DEF_REVERSE_VERTICAL_ICON_PATH));
	m_pActionReverseVertical->setCheckable(true);
	connect(m_pActionReverseVertical, &QAction::triggered,
		this, &Visuallica::on_m_pActionReverseVerticalTriggered);

	m_pActionReverseHorizontal = new QAction(tr("&Reverse Horizontal"), this);
	m_pActionReverseHorizontal->setIcon(QIcon(DEF_REVERSE_HORIZONTAL_ICON_PATH));
	m_pActionReverseHorizontal->setCheckable(true);
	connect(m_pActionReverseHorizontal, &QAction::triggered,
		this, &Visuallica::on_m_pActionReverseHorizontalTriggered);

	m_pActionRotateLeft = new QAction(tr("&Rotate 90 Left"), this);
	m_pActionRotateLeft->setIcon(QIcon(DEF_ROTATE_LEFT_ICON_PATH));
	m_pActionRotateLeft->setCheckable(true);
	connect(m_pActionRotateLeft, &QAction::triggered,
		this, &Visuallica::on_m_pActionRotateLeftTriggered);

	m_pActionRotateRight = new QAction(tr("&Rotate 90 Right"), this);
	m_pActionRotateRight->setIcon(QIcon(DEF_ROTATE_RIGHT_ICON_PATH));
	m_pActionRotateRight->setCheckable(true);
	connect(m_pActionRotateRight, &QAction::triggered,
		this, &Visuallica::on_m_pActionRotateRightTriggered);

	m_pActionRestoreMatrix = new QAction(tr("Restore Orientation"), this);
	m_pActionRestoreMatrix->setIcon(QIcon(DEF_RESTORE_MATRIX_ICON_PATH));
	m_pActionRestoreMatrix->setCheckable(true);
	connect(m_pActionRestoreMatrix, &QAction::triggered,
		this, &Visuallica::on_m_pActionRestoreMatrixTriggered);

	m_pActionMultiView = new QAction(tr("MultiView mode"), this);
	m_pActionMultiView->setIcon(QIcon(":/Icons/Resources/Icons/IconMultiView_128.png"));
	m_pActionMultiView->setCheckable(false);
	connect(m_pActionMultiView, &QAction::triggered,
		this, &Visuallica::on_m_pActionMultiViewTriggered);

	m_pActionFullScreen = new QAction(tr("Full screen view mode"), this);
	m_pActionFullScreen->setIcon(QIcon(":/Icons/Resources/Icons/IconFullScreen_128.png"));
	m_pActionFullScreen->setCheckable(false);	
	connect(m_pActionFullScreen, &QAction::triggered,
		this, &Visuallica::on_m_pActionFullScreenTriggered);

	m_pActionAbout = new QAction(tr("About Visuallica"), this);
	//m_pActionAbout->setCheckable(false);
	connect(m_pActionAbout, &QAction::triggered,
		this, &Visuallica::on_m_pActionAboutTriggered);


	m_pActionGroupBehaviorMode = new QActionGroup(this);
	
	m_pActionGroupBehaviorMode->addAction(m_pActionPan);
	m_pActionGroupBehaviorMode->addAction(m_pActionZoom);
	m_pActionGroupBehaviorMode->addAction(m_pActionPick);
	m_pActionGroupBehaviorMode->addAction(m_pActionOrgResolution);
	m_pActionGroupBehaviorMode->addAction(m_pActionFit);
	m_pActionGroupBehaviorMode->addAction(m_pActionReverseVertical);
	m_pActionGroupBehaviorMode->addAction(m_pActionReverseHorizontal);
	m_pActionGroupBehaviorMode->addAction(m_pActionRotateLeft);
	m_pActionGroupBehaviorMode->addAction(m_pActionRotateRight);
	m_pActionGroupBehaviorMode->addAction(m_pActionRestoreMatrix);

	m_pActionGroupBehaviorMode->setExclusive(true);
	m_pActionPick->setChecked(true);
	on_m_pActionPickTriggered();

	connect(&StateManager::GetInstance(), &StateManager::BehaviorModeStateChanged,
		this, &Visuallica::on_behaviorModeStateChanged);

}
void Visuallica::initMenubar()
{
	m_pMenuView = menuBar()->addMenu(tr("&View"));
	m_pMenuView->addAction(m_pActionOrgResolution);		
	m_pMenuView->addAction(m_pActionFit);
	m_pMenuView->addAction(m_pActionMultiView);
	m_pMenuView->addAction(m_pActionFullScreen);
	m_pMenuView->addSeparator();


	m_pMenuTools = menuBar()->addMenu(tr("&Tools"));
	m_pMenuTools->addAction(m_pActionPick);
	m_pMenuTools->addAction(m_pActionPan);
	m_pMenuTools->addAction(m_pActionZoom);
	m_pMenuTools->addAction(m_pActionWindowLevel);
	m_pMenuTools->addAction(m_pActionReverseVertical);
	m_pMenuTools->addAction(m_pActionReverseHorizontal);
	m_pMenuTools->addAction(m_pActionRotateLeft);
	m_pMenuTools->addAction(m_pActionRotateRight);
	m_pMenuTools->addAction(m_pActionRestoreMatrix);

	m_pMenuHelp = menuBar()->addMenu(tr("&Help"));
	m_pMenuHelp->addAction(m_pActionAbout);

}

void Visuallica::initToolbar()
{
	QSize sizeToobarIcon(40, 40);

	//ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(m_pActionPan);
	ui.mainToolBar->setIconSize(sizeToobarIcon);

	ui.mainToolBar->addAction(m_pActionZoom);
	ui.mainToolBar->setIconSize(sizeToobarIcon);

	ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(m_pActionOrgResolution);
	ui.mainToolBar->setIconSize(sizeToobarIcon);

	ui.mainToolBar->addAction(m_pActionFit);
	ui.mainToolBar->setIconSize(sizeToobarIcon);

	ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(m_pActionPick);
	ui.mainToolBar->setIconSize(sizeToobarIcon);

	ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(m_pActionWindowLevel);

	ui.mainToolBar->addSeparator();

	ui.mainToolBar->addAction(m_pActionReverseVertical);
	ui.mainToolBar->addAction(m_pActionReverseHorizontal);
	ui.mainToolBar->addAction(m_pActionRotateLeft);
	ui.mainToolBar->addAction(m_pActionRotateRight);
	ui.mainToolBar->addAction(m_pActionRestoreMatrix);

	ui.mainToolBar->addSeparator();
	ui.mainToolBar->addAction(m_pActionMultiView);
	ui.mainToolBar->addAction(m_pActionFullScreen);

}



void Visuallica::initWidgets()
{

}

void Visuallica::on_m_pActionZoomTriggered()
{
	qDebug() << "on_m_pActionZoomTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(ZOOM);
	m_pActionPick->setChecked(false);
	m_pActionPan->setChecked(false);
}

void Visuallica::on_m_pActionFitTriggered()
{
	qDebug() << "on_m_pActionFitTriggered() invoked!";
	//StateManager::GetInstance().SetViewMode(FITWINDOW);
	StateManager::GetInstance().SetBehaviorMode(FITWINDOW);
}

void Visuallica::on_m_pActionOrgResolutionTriggered()
{
	qDebug() << "on_m_pActionOrgResolutionTriggered() invoked!";
	//StateManager::GetInstance().SetViewMode(ORG_RESOLUTION);
	StateManager::GetInstance().SetBehaviorMode(ORG_RESOLUTION);
}

void Visuallica::on_m_pActionPickTriggered()
{
	qDebug() << "on_m_pActionPickTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(PICK);
}

void Visuallica::on_m_pActionPanTriggered()
{
	qDebug() << "on_m_pActionPanTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(PAN);
}

void Visuallica::on_m_pActionWindowLevelTriggered()
{
	qDebug() << "on_m_pActionWindowLevelTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(WINDOW);
}

void Visuallica::on_m_pActionReverseVerticalTriggered()
{
	qDebug() << "on_m_pActionReverseVerticalTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(REVERSE_VERTICAL);
}

void Visuallica::on_m_pActionReverseHorizontalTriggered()
{
	qDebug() << "on_m_pActionReverseHorizontalTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(REVERSE_HORIZONTAL);
}

void Visuallica::on_m_pActionRotateLeftTriggered()
{
	qDebug() << "on_m_pActionRotateLeftTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(ROTATE_LEFT);
}

void Visuallica::on_m_pActionRotateRightTriggered()
{
	qDebug() << "on_m_pActionRotateRightTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(ROTATE_RIGHT);
}

void Visuallica::on_m_pActionRestoreMatrixTriggered()
{
	qDebug() << "on_m_pActionRestoreMatrixTriggered() invoked!";
	StateManager::GetInstance().SetBehaviorMode(RESTORE_MATRIX);
}


void Visuallica::on_m_pActionMultiViewTriggered()
{
	qDebug() << "on_m_pActionMultiViewTriggered() invoked!";
	m_pMultiViewSelectionDlg = new MultiViewSelectDialog(this);
	m_pMultiViewSelectionDlg->show();
}

void Visuallica::on_m_pActionFullScreenTriggered()
{
	qDebug() << "on_m_pActionFullScreenTriggered() invoked!";
	
	if (!isFullScreen())
	{
		m_pTabLeft->setVisible(false);
		m_pThumbnailView->setVisible(false);

		m_bisMaximizedBeforeFullScreen = isMaximized();
		showFullScreen();
	}
	else
	{
		m_pTabLeft->setVisible(true);
		m_pThumbnailView->setVisible(true);

		if (m_bisMaximizedBeforeFullScreen)
			showMaximized(); 
		else
			showNormal();
	}
}

void Visuallica::on_m_pActionAboutTriggered()
{
	qDebug() << "on_m_pActionAboutTriggered() invoked!";


	QMessageBox::about(this, tr("About Visuallica"), "<a color=white href='codilizer@aol.com'><font size='12' color='white'>codilizer@aol.com</font></a>");
}

void Visuallica::on_behaviorModeStateChanged(E_BEHAVIORMODE eaBehaviorMode)
{
	m_pActionPick->setChecked(false);
	m_pActionPan->setChecked(false);
	m_pActionZoom->setChecked(false);
	m_pActionFit->setChecked(false);
	m_pActionWindowLevel->setChecked(false);
	m_pActionReverseVertical->setChecked(false);
	m_pActionReverseHorizontal->setChecked(false);
	m_pActionRotateLeft->setChecked(false);
	m_pActionRotateRight->setChecked(false);
	m_pActionRestoreMatrix->setChecked(false);

	if (PICK == eaBehaviorMode)
	{
		m_pActionPick->setChecked(true);
	}

	else if (PAN == eaBehaviorMode)
	{
		m_pActionPan->setChecked(true);
	}
	else if (ORG_RESOLUTION == eaBehaviorMode)
	{
		m_pActionPan->setChecked(true);
	}
	else if (FITWINDOW == eaBehaviorMode)
	{
		m_pActionPan->setChecked(true);
	}
	else if (ZOOM == eaBehaviorMode)
	{
		m_pActionZoom->setChecked(true);
	}
	else if (WINDOW == eaBehaviorMode)
	{
		m_pActionWindowLevel->setChecked(true);
	}
	else if (REVERSE_VERTICAL == eaBehaviorMode)
	{
		m_pActionReverseVertical->setChecked(true);
	}
	else if (REVERSE_HORIZONTAL == eaBehaviorMode)
	{
		m_pActionReverseHorizontal->setChecked(true);
	}
	else if (ROTATE_LEFT == eaBehaviorMode)
	{
		m_pActionRotateLeft->setChecked(true);
	}
	else if (ROTATE_RIGHT == eaBehaviorMode)
	{
		m_pActionRotateRight->setChecked(true);
	}
	else if (RESTORE_MATRIX == eaBehaviorMode)
	{
		m_pActionRestoreMatrix->setChecked(true);
	}

}