#include "MultiViewSelectTableView.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QDebug>

MultiViewSelectTableView::MultiViewSelectTableView(int aRow, int aColumn, QWidget* parent)
	:QTableView(parent)
{
	setMouseTracking(true);

	setFocusPolicy(Qt::NoFocus);
	setSelectionMode(QAbstractItemView::NoSelection);
	setWordWrap(true);
	setTabKeyNavigation(false);
	setCornerButtonEnabled(false);

	m_iRow = aRow;
	m_iColumn = aColumn;
	
	m_iSelctedRowCount = 0;
	m_iSelectedColumnCount = 0;

	m_pMultiViewSelectModel = new QStandardItemModel(m_iRow, m_iColumn);
	m_pItemSelectionModel = new QItemSelectionModel(m_pMultiViewSelectModel);
	
	initModel(m_pMultiViewSelectModel);
	setModel(m_pMultiViewSelectModel);

	setSize();
	
	m_bIsCellsSelected = false;
}

MultiViewSelectTableView::~MultiViewSelectTableView()
{
	if (m_pMultiViewSelectModel)
		delete m_pMultiViewSelectModel;

}

void MultiViewSelectTableView::initModel(QStandardItemModel* model)
{
	for (int row = 0; row < m_iRow; row++)
	{
		for (int column = 0; column < m_iColumn; column++)
		{
			if (m_pMultiViewSelectModel)
			{
				QStandardItem *item = new QStandardItem();
				model->setItem(row, column, item);
			}
		}
	}
}

void MultiViewSelectTableView::setSize()
{
	QHeaderView *indexHeader = verticalHeader();
	indexHeader->setVisible(false);

	QHeaderView *columnHeader = horizontalHeader();
	columnHeader->setVisible(false);

	horizontalHeader()->resizeSections(QHeaderView::Stretch);
	verticalHeader()->resizeSections(QHeaderView::Stretch);

	int w = 0, h = 0;


	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	w += contentsMargins().left()
		+ contentsMargins().right();
	h += contentsMargins().top()
		+ contentsMargins().bottom();

	for (int i = 0; i < columnHeader->count(); ++i)
		w += columnWidth(i);
	for (int i = 0; i < indexHeader->count(); ++i)
		h += rowHeight(i);


	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	setMinimumHeight(h);
	setMaximumHeight(h);

	setMinimumWidth(w);
	setMaximumWidth(w);

}

void MultiViewSelectTableView::ClearSelection()
{
	selectionModel()->clear();
	m_bIsCellsSelected = false;
	m_iSelctedRowCount = 0;
	m_iSelectedColumnCount = 0;

	for (int row = 0; row < m_iRow; row++)
	{
		for (int column = 0; column < m_iColumn; column++)
		{
			if (m_pMultiViewSelectModel)
			{
				m_pMultiViewSelectModel->item(row, column)->setSelectable(true);
			}
		}
	}

	emit CellsSelected(false);
}

void MultiViewSelectTableView::mousePressEvent(QMouseEvent * event)
{
	if (m_bIsCellsSelected == false)
	{
		m_bIsCellsSelected = true;
		emit CellsSelected(true);

		for (int row = 0; row < m_iRow; row++)
		{
			for (int column = 0; column < m_iColumn; column++)
			{
				if (row >= m_iSelctedRowCount || column >= m_iSelectedColumnCount)
				{
					// 선택 후 mouse hover시 페인팅 안되게 하려고 했는데 소용없음..
					m_pMultiViewSelectModel->item(row, column)->setSelectable(false); 
				}
			}
		}

	}
	return QTableView::mousePressEvent(event);
}

void MultiViewSelectTableView::mouseMoveEvent(QMouseEvent *event)
{
	if (false == m_bIsCellsSelected)
	{

		QPoint pos = event->globalPos();
		QPoint posFromTableView = viewport()->mapFromGlobal(pos);

		QModelIndex modelIndex = indexAt(posFromTableView);

		int hoveredRow = modelIndex.row();
		int hoveredColumn = modelIndex.column();

		if (hoveredRow + 1 == m_iSelctedRowCount && hoveredColumn + 1 == m_iSelectedColumnCount)
			return;

		selectionModel()->clear();

		for (int row = 0; row <= hoveredRow; row++)
		{
			for (int column = 0; column <= hoveredColumn; column++)
			{
				if (m_pMultiViewSelectModel)
				{
					QModelIndex index = m_pMultiViewSelectModel->index(row, column);
					selectionModel()->select(index, QItemSelectionModel::Select);
				}
			}
		}
		m_iSelctedRowCount = hoveredRow + 1;
		m_iSelectedColumnCount = hoveredColumn + 1;
		//return QTableView::mouseMoveEvent(event);
	}
}

void MultiViewSelectTableView::GetSelectedCount(int& aRow, int& aColumn)
{
	aRow = m_iSelctedRowCount;
	aColumn = m_iSelectedColumnCount;
}