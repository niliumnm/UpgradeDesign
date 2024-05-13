#include "tstandarditem.h"

TStandardItem::TStandardItem() {}

int TStandardItem::row() {
    if (m_Parent)
        return m_Parent -> m_lstChilds.indexOf(this);
    return 0;
}

int TStandardItem::childRows() {
    return m_lstChilds.count();
}

TStandardItem * TStandardItem::ParentItem() {
    return m_Parent;
}

void TStandardItem::RemoveItem(int row) {
    m_lstChilds.removeAt(row);
}

void TStandardItem::AddChild(TStandardItem * child) {
    child -> m_Parent = this;
    m_lstChilds.push_back(child);
}
