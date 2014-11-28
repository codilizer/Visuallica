#ifndef MULTIVIEWMANAGER_H
#define MULTIVIEWMANAGER_H

#include "Subject.h"
#include <QObject>

class MultiViewManager : public QObject, public Subject
{
	Q_OBJECT
public:
	static MultiViewManager& GetInstance()
	{
		static MultiViewManager instance;
		return instance;
	}

	virtual void Attach(Observer*);

	void AskChangeMultiView(int rowCount, int columnCount);
	void GetAskedMultiViewInfo(int& rowCount, int& columnCount);

signals:

private:
	MultiViewManager(QObject *parent = 0);
	~MultiViewManager();

	// Multi View
	int m_iAskedMultiViewRowCount;
	int m_iAskedMultiViewColumnCount;
};

#endif //MULTIVIEWMANAGER_H