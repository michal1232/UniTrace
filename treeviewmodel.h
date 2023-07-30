#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QStandardItemModel>
#include "DataViewList.h"

class TreeViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    TreeViewModel(QObject *parent = nullptr);


signals:
    void filterSignal(QString filtString, bool addToFilter);
    void expandChild(QModelIndex index);
    void colapsChild(QModelIndex index);
    void cleanModel();

public  slots:
    void insertGroup(QVector<GroupViewItem> *group);
    void filterSelected(const QModelIndex &index0, const QModelIndex &index1, const QModelIndex &index2, const QModelIndex &index3, bool checked);
    void expandSlot(bool expand);

private:
    QStandardItem *rootItem;

    void childFind(QStandardItem *item, bool expand);
    void childDelete(QStandardItem *item);
};

#endif // TREEVIEWMODEL_H
