#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include <QListView>
#include "Observer.h"
#include "QStringList"

class FileExplorer;
class ThumbnailsModel;

class ThumbnailView : public QListView, public Observer
{
	Q_OBJECT

public:
	ThumbnailView(FileExplorer *pSubjectFileExplorer, QWidget *parent = 0);
	~ThumbnailView();

	virtual void Update(Subject * theChangeSubject);

private:
	FileExplorer * m_pFileExplorer;
	
	ThumbnailsModel * m_pThumbnailsModel;
};

#endif // THUMBNAILVIEW_H