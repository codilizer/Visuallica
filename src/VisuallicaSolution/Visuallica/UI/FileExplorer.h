#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QTreeView>
#include "Subject.h"

class QFileSystemModel;
class QDirModel;

class FileExplorer : public QTreeView, public Subject
{
	Q_OBJECT

public:
	FileExplorer(QWidget *parent = 0);
	~FileExplorer();

	virtual void Attach(Observer*);

	QString GetSelectedDir();
	QStringList GetFiles();

protected:
	virtual void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);


private:

	QDirModel * m_pDirModel;
	QFileSystemModel * m_pFileModel;
	
	QScrollBar * m_pScrollBar;

	QString m_selectedDir;
	QStringList m_listFiles;
};

#endif // FILEEXPLORER_H