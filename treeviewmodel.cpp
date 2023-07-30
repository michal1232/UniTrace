#include "treeviewmodel.h"
#include <QStandardItem>
#include <QTreeView>

/**
 * @brief TreeViewModel::TreeViewModel
 * @param parent
 */
TreeViewModel::TreeViewModel(QObject *parent)
    : QStandardItemModel {parent}
{
    setColumnCount(1);
    rootItem = invisibleRootItem();
}

/**
 * @brief TreeViewModel::insertGroup
 * @param group
 */
void TreeViewModel::insertGroup(QVector<GroupViewItem> *group)
{
    QStandardItem *groupLevel0 = new QStandardItem;
    groupLevel0->setText("/");
    qint32 addNewSubCntNull = 0;


    for(qint32 i = 0; i < group->size(); i++)
    {
        QStandardItem *groupLevel1[group->at(i).groupVal.size()];
        bool addNewSubNull = true;


        if(i == 0) {
            groupLevel1[0] = new QStandardItem;
            groupLevel1[0]->setText(group->at(0).groupVal.at(0));
            groupLevel0->appendRow(groupLevel1[0]);
        }
        else {
            for(qint32 x = 0; x < i; x++) {
                if(group->at(x).groupVal.at(0) == group->at(i).groupVal.at(0)) {
                    addNewSubNull = false;
                    //addNewSubCntNull = x;
                    break;
                }
            }
            if(addNewSubNull == true) {
                groupLevel1[0] = new QStandardItem;
                groupLevel1[0]->setText(group->at(i).groupVal.at(0));
                groupLevel0->appendRow(groupLevel1[0]);
                addNewSubCntNull++;
            }
        }
        for(qint32 j = 1; j < group->at(i).groupVal.size(); j++)
        {
            if(addNewSubNull == true) {
                groupLevel1[j] = new QStandardItem;
                groupLevel1[j]->setText(group->at(i).groupVal.at(j));

                for(qint32 k = 0; k < group->at(i).groupReg.size(); k++)
                {
                    QStandardItem *regLevel1 = new QStandardItem;
                    regLevel1->setText(group->at(i).groupReg.at(k));
                    groupLevel1[j]->appendRow(regLevel1);
                }

                groupLevel1[j-1]->appendRow(groupLevel1[j]);
            }
            else {
                groupLevel1[j] = new QStandardItem;
                groupLevel1[j]->setText(group->at(i).groupVal.at(j));

                for(qint32 k = 0; k < group->at(i).groupReg.size(); k++)
                {
                    QStandardItem *regLevel1 = new QStandardItem;
                    regLevel1->setText(group->at(i).groupReg.at(k));
                    groupLevel1[j]->appendRow(regLevel1);
                }

                QStandardItem *child = new QStandardItem;
                child = groupLevel0->child(addNewSubCntNull);
                if(child != nullptr) {
                    child->appendRow(groupLevel1[j]);
                }
            }

//            for(qint32 k = 0; k < group->at(i).groupReg.size(); k++)
//            {
//                QStandardItem *regLevel1 = new QStandardItem;
//                regLevel1->setText(group->at(i).groupReg.at(k));
//                groupLevel1->appendRow(regLevel1);
//            }
        }


    }
    rootItem->removeRow(0);
   // childDelete(rootItem);

    rootItem->appendRow(groupLevel0);
}

void TreeViewModel::filterSelected(const QModelIndex &index0, const QModelIndex &index1, const QModelIndex &index2, const QModelIndex &index3, bool checked)
{
    QVariant val[4];
    QString filterString;

    val[3] = index0.data(Qt::DisplayRole);
    val[2] = index1.data(Qt::DisplayRole);
    val[1] = index2.data(Qt::DisplayRole);
    val[0] = index3.data(Qt::DisplayRole);

    for(qint32 i=0; i<4; i++) {

        filterString.append(val[i].toString());
        filterString.append(".");
    }
    filterString.remove(filterString.size()-1,1);
    QStringList splitFilterString = filterString.split("/.");

    if(checked == true) {
        if(splitFilterString.size() > 1)
            emit filterSignal(splitFilterString[1], true);
        else
            emit filterSignal("", true);
    }
    else
        if(splitFilterString.size() > 1)
            emit filterSignal(splitFilterString[1], false);
        else
            emit filterSignal("", false);
}

void TreeViewModel::expandSlot(bool expand)
{
    childFind(rootItem , expand);
}

void TreeViewModel::childFind(QStandardItem *item, bool expand)
{
    for(qint32 i = 0; i < item->rowCount(); i++) {
        if(expand == true)
            emit expandChild(item->child(i)->index());
        else
            emit colapsChild(item->child(i)->index());

        if(item->child(i)->hasChildren()) {
            childFind(item->child(i), expand);
        }
    }
}

void TreeViewModel::childDelete(QStandardItem *item)
{
    for(qint32 i = 0; i < item->rowCount(); i++) {
        qInfo() << item->child(i)->text();
        item->child(i)->removeRow(i);

        if(item->child(i)->hasChildren()) {
            childDelete(item->child(i));
        }
    }
}

