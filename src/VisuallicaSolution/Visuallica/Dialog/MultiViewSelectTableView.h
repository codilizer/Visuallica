#ifndef MULTIVIEWSELECTTABLEVIEW_H
#define MULTIVIEWSELECTTABLEVIEW_H

#include <QTableView>

class QMouseEvent;
class QStandardItemModel;
class QItemSelectionModel;

class MultiViewSelectTableView : public QTableView
{
	Q_OBJECT

public:
	MultiViewSelectTableView(int, int, QWidget* parent = 0);
	~MultiViewSelectTableView();

	virtual void mousePressEvent(QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent *event);

	void ClearSelection();
	void GetSelectedCount(int& selectedRowCount, int& selectedColumnCount);

signals:
	void CellsSelected(bool);
	

private:
	void initModel(QStandardItemModel*);
	void setSize();
	void setSelectable(bool);

	QStandardItemModel* m_pMultiViewSelectModel;
	QItemSelectionModel* m_pItemSelectionModel;

	bool m_bIsCellsSelected;


	int m_iRow;
	int m_iColumn;

	int m_iSelctedRowCount;
	int m_iSelectedColumnCount;

};




#endif // MULTIVIEWSELECTTABLEVIEW_H