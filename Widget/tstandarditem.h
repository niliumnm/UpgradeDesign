#ifndef TSTANDARDITEM_H
#define TSTANDARDITEM_H

#include <QObject>
#include <QStandardItem>
#include <QList>
#include <QVariant>

class TStandardItem :public QObject,public QStandardItem
{
    Q_OBJECT
public:
    TStandardItem();
    int row();
    int childRows();
    TStandardItem *ParentItem();
    void RemoveItem(int row);
    void AddChild(TStandardItem * child);

private:
    QList<TStandardItem*> m_lstChilds;
    QVariant m_Val;
    TStandardItem* m_Parent;
};



#endif // TSTANDARDITEM_H
