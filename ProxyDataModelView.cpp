#include "ProxyDataModelView.h"
#include "QDebug"

ProxyDataModelView::ProxyDataModelView(QObject *parent)
    : QSortFilterProxyModel(parent),
      m_complete(false)
{
    connect(this, &QSortFilterProxyModel::rowsInserted, this, &ProxyDataModelView::countChanged);
    connect(this, &QSortFilterProxyModel::rowsRemoved, this, &ProxyDataModelView::countChanged);
}

int ProxyDataModelView::count() const
{
    return rowCount();
}

QObject *ProxyDataModelView::source() const
{
    return sourceModel();
}

void ProxyDataModelView::setSource(QObject *source)
{
    setSourceModel(qobject_cast<QAbstractItemModel *>(source));
}

QByteArray ProxyDataModelView::sortRole() const
{
    return m_sortRole;
}

void ProxyDataModelView::setSortRole(const QByteArray &role)
{
    if (m_sortRole != role) {
        m_sortRole = role;
        if (m_complete)
            QSortFilterProxyModel::setSortRole(roleKey(role));
    }
}

void ProxyDataModelView::setSortOrder(Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(0, order);
}

QByteArray ProxyDataModelView::filterRole() const
{
    return m_filterRole;
}

void ProxyDataModelView::setFilterRole(const QByteArray &role)
{
    if (m_filterRole != role) {
        m_filterRole = role;
        if (m_complete)
            QSortFilterProxyModel::setFilterRole(roleKey(role));
    }
}

QString ProxyDataModelView::filterString() const
{
    return filterRegExp().pattern();
}

void ProxyDataModelView::setFilterString(const QString &filter)
{
    setFilterRegExp(QRegExp(filter, filterCaseSensitivity(), static_cast<QRegExp::PatternSyntax>(filterSyntax())));
}

ProxyDataModelView::FilterSyntax ProxyDataModelView::filterSyntax() const
{
    return static_cast<FilterSyntax>(filterRegExp().patternSyntax());
}

void ProxyDataModelView::setFilterSyntax(ProxyDataModelView::FilterSyntax syntax)
{
    setFilterRegExp(QRegExp(filterString(), filterCaseSensitivity(), static_cast<QRegExp::PatternSyntax>(syntax)));
}

void ProxyDataModelView::classBegin()
{
}

void ProxyDataModelView::componentComplete()
{
    m_complete = true;
    if (!m_sortRole.isEmpty())
        QSortFilterProxyModel::setSortRole(roleKey(m_sortRole));
    if (!m_filterRole.isEmpty())
        QSortFilterProxyModel::setFilterRole(roleKey(m_filterRole));
}

int ProxyDataModelView::roleKey(const QByteArray &role) const
{
    return roleNames().key(role, -1);
}

QHash<int, QByteArray> ProxyDataModelView::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel())
        return source->roleNames();
    return QHash<int, QByteArray>();
}


bool ProxyDataModelView::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{    
    static QVector<int> inRow;

    QRegExp rx = filterRegExp();
    if (rx.isEmpty())
        return true;

    QString pattern = rx.pattern();
    QStringList patternList;

    if(pattern.contains("|")) {
        patternList = pattern.split("|");
        if(patternList[0] == "") {
            patternList.removeFirst();
        }
    }
    else {
        patternList.append(pattern);
    }
    //rx.setPatternSyntax(QRegExp::Wildcard);

    QAbstractItemModel *model = sourceModel();
    if (filterRole().isEmpty()) {
        QHash<int, QByteArray> roles = roleNames();
        for (auto it = roles.cbegin(), end = roles.cend(); it != end; ++it) {
            QModelIndex sourceIndex = model->index(sourceRow, 0, sourceParent);
            QString key = model->data(sourceIndex, it.key()).toString();
            for(qint32 i = 0; i < patternList.size(); i++) {
                if (key.contains(patternList[i]) || patternList[i].contains('*')) {
                    if(inRow.contains(sourceRow) == false) {

                        for(int i = inRow.size(); i > 0; i--) {
                            if(inRow.at(i-1) > sourceRow) {
                                if(i >= 2) {
                                    if(inRow.at(i-2) < sourceRow) {
                                        inRow.insert(i-1,sourceRow);
                                        emit rowChanged(i-1, sourceRow);
                                        break;
                                    }
                                }
                                else {
                                    inRow.insert(i-1,sourceRow);
                                    emit rowChanged(i-1, sourceRow);
                                    break;
                                }
                            }
                            else {
                                inRow.insert(i,sourceRow);
                                emit rowChanged(i, sourceRow);
                                break;
                            }
                        }
                        if(inRow.size() == 0) {
                            inRow.append(sourceRow);
                            emit rowChanged(0, sourceRow);
                        }
                    }

                    return true;
                }

            }
       }
        for(int i=0; i<inRow.size();i++) {
            if(inRow.at(i) == sourceRow) {
                inRow.removeAt(i);
                emit rowDeleted(i);
            }
        }

        return false;
    }
    QModelIndex sourceIndex = model->index(sourceRow, 0, sourceParent);
    if (!sourceIndex.isValid())
        return true;
    QString key = model->data(sourceIndex, roleKey(filterRole())).toString();
    return key.contains(rx);
}
