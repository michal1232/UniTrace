#ifndef PROXYDATAMODELVIEW_H
#define PROXYDATAMODELVIEW_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QtQml/qqmlparserstatus.h>
#include <QtQml/qjsvalue.h>

class ProxyDataModelView : public QSortFilterProxyModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QObject *source READ source WRITE setSource)

    Q_PROPERTY(QByteArray sortRole READ sortRole WRITE setSortRole)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder)

    Q_PROPERTY(QByteArray filterRole READ filterRole WRITE setFilterRole)
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString)
    Q_PROPERTY(FilterSyntax filterSyntax READ filterSyntax WRITE setFilterSyntax)

    Q_ENUMS(FilterSyntax)
public:
    explicit ProxyDataModelView(QObject *parent = nullptr);

    QObject *source() const;
    void setSource(QObject *source);

    QByteArray sortRole() const;
    void setSortRole(const QByteArray &role);

    void setSortOrder(Qt::SortOrder order);

    QByteArray filterRole() const;
    void setFilterRole(const QByteArray &role);

    QString filterString() const;
    void setFilterString(const QString &filter);

    enum FilterSyntax {
        RegExp,
        Wildcard,
        FixedString
    };

    FilterSyntax filterSyntax() const;
    void setFilterSyntax(FilterSyntax syntax);

    int count() const;
    void classBegin();
    void componentComplete();

signals:
    void countChanged();
    void rowChanged(qint32 position, qint32 value) const;
    void rowDeleted(qint32 position) const;

protected:
    int roleKey(const QByteArray &role) const;
    QHash<int, QByteArray> roleNames() const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    bool m_complete;
    QByteArray m_sortRole;
    QByteArray m_filterRole;

};

#endif // PROXYDATAMODELVIEW_H
