#ifndef TSTANDARDITEMMODEL_H
#define TSTANDARDITEMMODEL_H

#include <QObject>
#include <QStandardItemModel>

class TStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TStandardItemModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    // QAbstractItemModel interface
public:
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
    virtual Qt::DropActions supportedDropActions() const override;
};



#endif // TSTANDARDITEMMODEL_H
