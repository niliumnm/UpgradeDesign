#include "tstandarditemmodel.h"

#include<QIODevice>

#include<QMimeData>

#include<Widget/tstandarditem.h>

TStandardItemModel::TStandardItemModel(QObject * parent): QStandardItemModel {
        parent
    } {

}
QMimeData * TStandardItemModel::mimeData(const QModelIndexList & indexes) const {
    QMimeData * mimeData = QAbstractItemModel::mimeData(indexes);
    //只取第一个
    for (int i = 0; i < indexes.count(); i++) {
        QModelIndex index = indexes[i];
        QModelIndex * p = new QModelIndex(index);
        TStandardItem * item = static_cast < TStandardItem * > (index.internalPointer());
        //qDebug() << item->data(Qt::DisplayRole).toString();

        QByteArray array;
        QDataStream stream( & array, QIODevice::WriteOnly);
        stream << (qint64) p;
        mimeData -> setData(QString("hehe"), array);
        return mimeData;
    }
    return mimeData;
}

Qt::DropActions TStandardItemModel::supportedDropActions() const {
    return Qt::MoveAction;
}

bool TStandardItemModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column,
                                      const QModelIndex & parent) {
    QByteArray array = data -> data(QString("hehe"));
    QDataStream stream( & array, QIODevice::ReadOnly);
    qint64 p;
    stream >> p;
    QModelIndex * index = (QModelIndex * ) p;

    TStandardItem * item = static_cast < TStandardItem * > (index -> internalPointer());
    TStandardItem * ParentItem = static_cast < TStandardItem * > (parent.internalPointer());

    beginMoveRows(index -> parent(), index -> row(), index -> row(), parent, ParentItem -> childRows());
    item -> ParentItem() -> RemoveItem(item -> row());
    ParentItem -> AddChild(item);
    endMoveRows();

    delete index;

    return false;
}
