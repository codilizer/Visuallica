#include "FileExplorer.h"
#include "QFileSystemModel.h"
#include "QDirModel"
#include "QDebug.h"
#include "QHeaderView.h"

//#include <QtCore>

FileExplorer::FileExplorer(QWidget *parent)
	: QTreeView(parent)
{
	m_pDirModel = new QDirModel(this);
	
	m_pDirModel->setReadOnly(false);
	m_pDirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase);

	setModel(m_pDirModel);

	int columncount = m_pDirModel ->columnCount();

	for (int i = 0; i < columncount; i++)
	{
		if (i > 0) {
			setColumnHidden(i, true);
		}
	}

	setTextElideMode(Qt::ElideNone);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	header()->setStretchLastSection(false);
}

FileExplorer::~FileExplorer()
{


}

void FileExplorer::Attach(Observer* pObserver)
{
	Subject::Attach(pObserver);

	Notify();
}


void FileExplorer::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{

	qDebug() << selected.size();

	// 아래와 같이 하면 하나만 선택시에도 인데스가 여러개 리스트로 넘어온다.
	//foreach(QModelIndex index, selected.indexes())
	//{
	//	qDebug() << m_pDirModel->filePath(index);
	//	qDebug() << m_pDirModel->fileName(index);
	//}

	qDebug() << m_pDirModel->filePath(selectionModel()->currentIndex());
	qDebug() << m_pDirModel->fileName(selectionModel()->currentIndex());	

	
	QDir directory(m_pDirModel->filePath(selectionModel()->currentIndex()));

	m_selectedDir = directory.canonicalPath();

	QStringList nameFilter("*.*");
	m_listFiles = directory.entryList(nameFilter, QDir::Files | QDir::NoDotAndDotDot);

	qDebug() << m_selectedDir;
	qDebug() << m_listFiles;

	Notify();
}

QString FileExplorer::GetSelectedDir()
{
	return m_selectedDir;

}

QStringList FileExplorer::GetFiles()
{
	return m_listFiles;
}

