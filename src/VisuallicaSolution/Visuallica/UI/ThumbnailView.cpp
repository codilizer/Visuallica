#include "ThumbnailView.h"
#include "FileExplorer.h"
#include "ThumbnailsModel.h"
#include "QImage.h"
#include "QLabel.h"
#include <QDir>
#include <QScroller>

ThumbnailView::ThumbnailView(FileExplorer *pSubjectFileExplorer, QWidget *parent)
	: QListView(parent), Observer()
{

	setDragEnabled(true);
	setViewMode(QListView::IconMode);
	setIconSize(QSize(200, 200));
	setSpacing(10);
	setMovement(QListView::Free);
	setLayoutMode(QListView::SinglePass);
	setDropIndicatorShown(true);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollMode(ScrollMode::ScrollPerPixel);
	
	setWrapping(false);

	setAcceptDrops(false);

	m_pThumbnailsModel = new ThumbnailsModel(this);
	setModel(m_pThumbnailsModel);

	m_pFileExplorer = pSubjectFileExplorer;
	m_pFileExplorer->Attach(this);

	QScroller::grabGesture(this, QScroller::RightMouseButtonGesture);

}

ThumbnailView::~ThumbnailView()
{
	m_pFileExplorer->Detach(this);
}

void ThumbnailView::Update(Subject *pChangedSubject)
{
	if (pChangedSubject == m_pFileExplorer)
	{
		QString selectedDir = m_pFileExplorer->GetSelectedDir();

		m_pThumbnailsModel->setThumbnailFileList(selectedDir);
	}

}