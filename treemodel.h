#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class TreeItem ;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent = 0);
	~TreeModel() ;

	QVariant data(const QModelIndex &index, int role) const ;
	int rowCount(const QModelIndex &parent) const ;
	int columnCount(const QModelIndex &parent) const ;
	Qt::ItemFlags flags(const QModelIndex &index) const ;
	bool setData(const QModelIndex &index, const QVariant &value, int role) ;
	bool insertRows(int row, int count, const QModelIndex &parent) ;
	bool removeRows(int row, int count, const QModelIndex &parent) ;
	QModelIndex index(int row, int column, const QModelIndex &parent) const ;
	QModelIndex parent(const QModelIndex &child) const ;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const ;

	void addTree(QString &str, QModelIndex &parent) ;
	void removeTree(QModelIndex &parent) ;
signals:

public slots:

private:
	TreeItem	*m_pRootItem ;
};

#endif // TREEMODEL_H
