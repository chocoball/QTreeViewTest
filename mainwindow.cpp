#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	m_pTreeModel = new TreeModel(this) ;
	ui->treeView->setModel(m_pTreeModel) ;

	// drag and drop 追加
	ui->treeView->setDragEnabled(true) ;
	ui->treeView->setAcceptDrops(true) ;
	ui->treeView->setDropIndicatorShown(true) ;
	ui->treeView->setDragDropMode(QAbstractItemView::InternalMove) ;
	// ------------------

	connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(slot_clickAdd())) ;
	connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(slot_clickDel())) ;
	connect(ui->pushButton_dump, SIGNAL(clicked()), this, SLOT(slot_clickDump())) ;
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
	ui->treeView->expand(m_selIndex);		// ツリーを展開する
}

void MainWindow::slot_clickDel()
{
	m_pTreeModel->removeTree(m_selIndex) ;
	m_selIndex = QModelIndex() ;
	ui->treeView->setCurrentIndex(m_selIndex);
}

void MainWindow::slot_clickDump()
{
	m_pTreeModel->dumpTreeItems() ;
}

void MainWindow::slot_clickTree(QModelIndex index)
{
	m_selIndex = index ;
}
