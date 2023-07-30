#ifndef CODELAYER_H
#define CODELAYER_H

#include <QObject>
#include <QTextDocument>
#include <QSyntaxHighlighter>
#include <QQuickTextDocument>

enum CODE_LINE_TYPE{
   CODE_LINE = 1,
   LABEL_LINE
};

class TextCharFormat : public QObject, public QTextCharFormat
{
    Q_OBJECT

    Q_PROPERTY (QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY (QVariant foreground READ foreground WRITE setForeground NOTIFY foregroundChanged)

public:
    TextCharFormat(QObject* parent = nullptr);

signals:
    void fontChanged();
    void foregroundChanged();

protected:
    void setFont(const QFont& font);
    QFont font() const;

    QVariant foreground() const;
    void setForeground(const QVariant& foreground);

};

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument* textDocument READ textDocument WRITE setTextDocument NOTIFY textDocumentChanged)

public:
    SyntaxHighlighter(QObject* parent = nullptr);

    Q_INVOKABLE void setFormat(int start, int count, const QVariant& format);

signals:
    void textDocumentChanged();
    void highlightBlock(const QVariant& text);

protected:
    QQuickTextDocument* m_TextDocument;

    QQuickTextDocument* textDocument() const;
    void setTextDocument(QQuickTextDocument* textDocument);

    virtual void highlightBlock(const QString &text);

};


class codeLayer:public QObject
{
    Q_OBJECT
public:
    codeLayer();

signals:
    void addLineToCodeLayout(qint32 page, qint32 line, QString code, qint32 type);
    void reloadCodeDesigne(qint32 maxLine);

public slots:
    void addLineToLayerSlot(qint32 page, qint32 line, qint32 type);
    void addTextToLineSlot(qint32 page, qint32 line, QString code);
    void deleteLineFromLayerSlot(qint32 page, qint32 line);
    void loadCodePageSlot(qint32 page);
    void saveCodeLayer(QString fileSource, qint32 page);
    void loadCodeLayer(QString fileSource, qint32 page);
private:
    qint32 maxPage;
    qint32 linePage[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    QVector<QString> textLine[10];
    QVector<qint32> typeLine[10];
};

#endif // CODELAYER_H
