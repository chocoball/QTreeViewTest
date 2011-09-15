#include "treemodel.h"

class TreeItem
{
public:
	TreeItem(QString str, TreeItem *parent) :
		m_str(str), m_pParent(parent)
	{
	}
	~TreeItem()
	{
		qDeleteAll(m_children) ;
	}
	void appendChild(TreeItem *pChild) { m_children.append(pChild) ; }
	void insertChild(int idx, TreeItem *pChild) { m_children.insert(idx, pChild) ; }
	void removeChild(TreeItem *p)
	{
		int index = m_children.indexOf(p) ;
		if ( index < 0 ) { return ; }
		m_children.removeAt(index) ;
		delete p ;
	}

	QString data() { return m_str ; }
	void setData(QString str) { m_str = str ; }
	int childCount() { return m_children.size() ; }
	QList<TreeItem *> &children() { return m_children ; }
	TreeItem *parent() { return m_pParent ; }
	TreeItem *child(int row)
	{
		if ( row < 0 || row >= childCount() ) { return NULL ; }
		return m_children[row] ;
	}
	int row()
	{
		if ( m_pParent ) {
			return m_pParent->children().indexOf(this) ;
		}
		return 0 ;
	}

private:
	QString				m_str ;
	TreeItem			*m_pParent ;
	QList<TreeItem *>	m_children ;
} ;


TreeModel::TreeModel(QObject *parent) :
	QAbstractItemModel(parent)
{
	m_pRootItem = new TreeItem("header", NULL) ;

	// 初期アイテム 追加してみる
	m_pRootItem->appendChild(new TreeItem(QString("aaa"), m_pRootItem));
	TreeItem *p = new TreeItem(QString("bbb"), m_pRootItem) ;
	m_pRootItem->appendChild(p) ;
	p->appendChild(new TreeItem(QString("ccc"), p)) ;
}

TreeModel::~TreeModel()
{
	delete m_pRootItem ;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if ( role != Qt::DisplayRole && role != Qt::EditRole ) { return QVariant() ; }
	if ( !index.isValid() ) { return QVariant() ; }

	TreeItem *p = static_cast<TreeItem *>(index.internalPointer()) ;
	return p->data() ;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *p = m_pRootItem ;
	if ( parent.isValid() ) {
		p = static_cast<TreeItem *>(parent.internalPointer()) ;
	}

	return p->childCount() ;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	return 1 ;	// カラムは常に1つ
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if ( !index.isValid() ) {
		return Qt::ItemIsEnabled ;
	}
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable ;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if ( role != Qt::DisplayRole && role != Qt::EditRole ) {
		return false ;
	}

	TreeItem *p = m_pRootItem ;
	if ( index.isValid() ) {
		p = static_cast<TreeItem *>(index.internalPointer()) ;
	}

	p->setData(value.toString()) ;
	emit dataChanged(index, index);
	return true ;
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row+count-1) ;
	TreeItem *p = m_pRootItem ;
	if ( parent.isValid() ) {
		p = static_cast<TreeItem *>(parent.internalPointer()) ;
	}

	p->insertChild(row, new TreeItem(QString(), p)) ;
	endInsertRows();
	return true ;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row+count-1) ;
	TreeItem *p = m_pRootItem ;
	if ( parent.isValid() ) {
		p = static_cast<TreeItem *>(parent.internalPointer()) ;
	}

	p->removeChild(p->child(row)) ;
	endRemoveRows();
	return true ;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if ( !hasIndex(row, column, parent) ) { return QModelIndex() ; }

	TreeItem *p = m_pRootItem ;
	if ( parent.isValid() ) {
		p = static_cast<TreeItem *>(parent.internalPointer()) ;
	}

	TreeItem *pChild = p->child(row) ;
	if ( pChild ) {
		return createIndex(row, column, pChild) ;
	}
	return QModelIndex() ;
}

QModelIndex TreeModel::parent(const QModelIndex &child) const
{
	if ( !child.isValid() ) { return QModelIndex() ; }
	TreeItem *c = static_cast<TreeItem *>(child.internalPointer()) ;
	TreeItem *p = c->parent() ;
	if ( p == m_pRootItem ) { return QModelIndex() ; }
	return createIndex(p->row(), 0, p) ;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if ( orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0 ) {
		return m_pRootItem->data() ;
	}
	return QVariant() ;
}

void TreeModel::addTree(QString &str, QModelIndex &parent)
{
	TreeItem *p = m_pRootItem ;
	if ( parent.isValid() ) {
		p = static_cast<TreeItem *>(parent.internalPointer()) ;
	}
	int row = p->childCount() ;

	insertRows(row, 1, parent) ;	// row 追加

	QModelIndex index = this->index(row, 0, parent) ;
	setData(index, str, Qt::DisplayRole) ;
}

void TreeModel::removeTree(QModelIndex &index)
{
	if ( !index.isValid() ) { return ; }

	removeRows(index.row(), 1, index.parent()) ;
}

