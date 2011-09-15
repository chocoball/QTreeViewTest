#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "treemodel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
	void slot_clickAdd() ;
	void slot_clickDel() ;
	void slot_clickTree(QModelIndex index) ;

private:
    Ui::MainWindow *ui;
	TreeModel		*m_pTreeModel ;
	QModelIndex		m_selIndex ;
};

#endif // MAINWINDOW_H
