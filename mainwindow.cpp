#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	m_pTreeModel = new TreeModel(this) ;
	ui->treeView->setModel(m_pTreeModel) ;

	connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(slot_clickAdd())) ;
	connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(slot_clickDel())) ;
	connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clickTree(QModelIndex))) ;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slot_clickAdd()
{
	static int count = 0 ;
	count ++ ;
	QString str = QString("%1").arg(count) ;
	m_pTreeModel->addTree(str, m_selIndex);
}

void MainWindow::slot_clickDel()
{
	m_pTreeModel->removeTree(m_selIndex) ;
	m_selIndex = QModelIndex() ;
	ui->treeView->setCurrentIndex(m_selIndex);
}

void MainWindow::slot_clickTree(QModelIndex index)
{
	qDebug() << "slot_clickTree valid:" << index.isValid() << " row:" << index.row() ;
	m_selIndex = index ;
}
