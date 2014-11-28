#ifndef MULTIVIEWSELECTDIALOG_H
#define MULTIVIEWSELECTDIALOG_H

#include <QDialog>


class QTableView;
class QStandardItemModel;

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class MultiViewSelectTableView;
class QCloseEvent;

class MultiViewSelectDialog : public QDialog
{
	Q_OBJECT

public:
	MultiViewSelectDialog(QWidget* parent = 0);
	~MultiViewSelectDialog();

public slots:
	void on_m_pBtnApplyClicked();
	void on_m_pBtnClearSelectionClicked();
	void on_m_pBtnCancelClicked();

	void on_CellsSelected(bool);

private:
	void initLayout();

	QVBoxLayout *m_pVBoxLayoutMain;
	QHBoxLayout *m_pHBoxLayoutNotification;
	QHBoxLayout *m_pHBoxLayoutSelectTableView;
	QHBoxLayout *m_pHBoxLayoutButtons;
	
	QLabel *m_pLabelNotification;

	MultiViewSelectTableView * m_pTableViewMultiViewSelect;
	
	
	QPushButton * m_pBtnApply;
	QPushButton * m_pBtnSelectionClear;
	QPushButton * m_pBtnCancel;
};


#endif // MULTIVIEWSELECTDIALOG_H