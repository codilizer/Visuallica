#include "MultiViewSelectDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "MultiViewSelectTableView.h"
#include <QStandardItemModel>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QMouseEvent>
#include <QDebug>
#include "CommonDefines.h"
#include "MultiViewManager.h"

MultiViewSelectDialog::MultiViewSelectDialog(QWidget *parent)
	: QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setModal(true);

	// Remove question mark
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

	initLayout();

}

MultiViewSelectDialog::~MultiViewSelectDialog()
{
	qDebug() << "~MultiViewSelectDialog() invoked";
}

void MultiViewSelectDialog::initLayout()
{
	m_pVBoxLayoutMain = new QVBoxLayout(this);
	m_pHBoxLayoutNotification = new QHBoxLayout(this);
	m_pHBoxLayoutSelectTableView = new QHBoxLayout(this);
	m_pHBoxLayoutButtons = new QHBoxLayout(this);

	setLayout(m_pVBoxLayoutMain);

	m_pVBoxLayoutMain->addLayout(m_pHBoxLayoutNotification);

	m_pLabelNotification = new QLabel(tr("Please select cell(s) for disply."), this);
	m_pHBoxLayoutNotification->addWidget(m_pLabelNotification);

	m_pVBoxLayoutMain->addLayout(m_pHBoxLayoutSelectTableView);

	m_pTableViewMultiViewSelect = new MultiViewSelectTableView(MAX_GRAPHICS_VIEW_ROW_COUNT, MAX_GRAPHICS_VIEW_COLUMN_COUNT, this);
	connect(m_pTableViewMultiViewSelect, &MultiViewSelectTableView::CellsSelected,
		this, &MultiViewSelectDialog::on_CellsSelected);
	

	m_pHBoxLayoutSelectTableView->addWidget(m_pTableViewMultiViewSelect);
	
	m_pVBoxLayoutMain->addLayout(m_pHBoxLayoutButtons);

	m_pBtnApply = new QPushButton(tr("Apply"), this);
	m_pBtnApply->setEnabled(false);
	connect(m_pBtnApply, &QPushButton::clicked,
		this, &MultiViewSelectDialog::on_m_pBtnApplyClicked);

	m_pBtnSelectionClear = new QPushButton(tr("Selection Clear"), this);
	m_pBtnSelectionClear->setEnabled(false);
	connect(m_pBtnSelectionClear, &QPushButton::clicked,
		this, &MultiViewSelectDialog::on_m_pBtnClearSelectionClicked);


	m_pBtnCancel = new QPushButton(tr("Cancel"), this);
	connect(m_pBtnCancel, &QPushButton::clicked,
		this, &MultiViewSelectDialog::on_m_pBtnCancelClicked);



	m_pHBoxLayoutButtons->addWidget(m_pBtnApply, 0, Qt::AlignRight);
	m_pHBoxLayoutButtons->addWidget(m_pBtnSelectionClear, 0, Qt::AlignRight);
	m_pHBoxLayoutButtons->addWidget(m_pBtnCancel, 0, Qt::AlignRight);

}

void MultiViewSelectDialog::on_CellsSelected(bool isSelected)
{
	m_pBtnSelectionClear->setEnabled(isSelected);
	m_pBtnApply->setEnabled(isSelected);

}


void MultiViewSelectDialog::on_m_pBtnApplyClicked()
{
	//Todo

	int selectedRowCount = 0;
	int selectedColumnCount = 0;

	m_pTableViewMultiViewSelect->GetSelectedCount(selectedRowCount, selectedColumnCount);

	MultiViewManager::GetInstance().AskChangeMultiView(selectedRowCount, selectedColumnCount);
	this->close();
}

void MultiViewSelectDialog::on_m_pBtnClearSelectionClicked()
{
	if (m_pTableViewMultiViewSelect)
		m_pTableViewMultiViewSelect->ClearSelection();
}

void MultiViewSelectDialog::on_m_pBtnCancelClicked()
{
	this->close();
}

