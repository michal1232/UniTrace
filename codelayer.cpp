#include "codelayer.h"
#include <QXmlStreamReader>
#include <QDebug>

TextCharFormat::TextCharFormat(QObject* parent) :
    QObject(parent)
{
}

void TextCharFormat::setFont( const QFont& font )
{
    if ( font == QTextCharFormat::font() )
    {
        return;
    }

    QTextCharFormat::setFont(font);
    emit fontChanged();
}

QFont TextCharFormat::font() const
{
    return QTextCharFormat::font();
}

QVariant TextCharFormat::foreground() const
{
    return QTextCharFormat::foreground().color();
}

void TextCharFormat::setForeground( const QVariant& foreground )
{
    if ( foreground.canConvert<QColor>() )
    {
        QTextCharFormat::setForeground( QBrush( foreground.value< QColor >() ) );
        emit foregroundChanged();
    }
}

SyntaxHighlighter::SyntaxHighlighter( QObject* parent ) :
    QSyntaxHighlighter(parent),
    m_TextDocument(nullptr)
{
}

void SyntaxHighlighter::highlightBlock( const QString &text )
{
    emit highlightBlock( QVariant(text) );
}

QQuickTextDocument* SyntaxHighlighter::textDocument() const
{
    return m_TextDocument;
}

void SyntaxHighlighter::setTextDocument( QQuickTextDocument* textDocument )
{
    if (textDocument == m_TextDocument)
    {
        return;
    }

    m_TextDocument = textDocument;

    QTextDocument* doc = m_TextDocument->textDocument();
    setDocument(doc);

    emit textDocumentChanged();
}

void SyntaxHighlighter::setFormat( int start, int count, const QVariant& format )
{
    TextCharFormat* charFormat = qvariant_cast<TextCharFormat*>( format );
    if ( charFormat )
    {
        QSyntaxHighlighter::setFormat( start, count, *charFormat );
        return;
    }

    if ( format.canConvert(QVariant::Color) )
    {
        QSyntaxHighlighter::setFormat( start, count, format.value<QColor>() );
        return;
    }

    if ( format.canConvert(QVariant::Font) )
    {
        QSyntaxHighlighter::setFormat( start, count, format.value<QFont>() );
        return;
    }
}

codeLayer::codeLayer()
{
    maxPage = 1;
}

void codeLayer::addLineToLayerSlot(qint32 page, qint32 line, qint32 type)
{
    if(page < maxPage) {
        textLine[page].append("");
        linePage[page] = line;
        typeLine[page].append(type);
    }
}

void codeLayer::addTextToLineSlot(qint32 page, qint32 line, QString code)
{
    if(page < maxPage && line < linePage[page]) {
        textLine[page][line].clear();
        textLine[page][line].append(code);
    }
}

void codeLayer::deleteLineFromLayerSlot(qint32 page, qint32 line)
{
    if(page < maxPage && line < linePage[page]) {
        linePage[page]--;
        textLine[page].removeAt(line);
        typeLine[page].removeAt(line);
        emit reloadCodeDesigne(line);
    }
}

void codeLayer::loadCodePageSlot(qint32 page)
{
    if(page < maxPage) {
        for(qint32 i = 0; i < linePage[page]; i++) {
            emit addLineToCodeLayout(page, i, textLine[page].at(i), typeLine[page].at(i));
        }
    }
}

void codeLayer::saveCodeLayer(QString fileSource, qint32 page)
{
    QFile file(fileSource);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);

        stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        stream << "<CodeData>\n";


       for (int c=0; c < linePage[page]; c++ ) {
           stream <<  "<code title=\""<<typeLine[page].at(c) << "\">" <<textLine[page].at(c) <<  "</code>\n";
       }

       stream << "</CodeData>\n";

    }
}

void codeLayer::loadCodeLayer(QString fileSource, qint32 page)
{
    QFile file(fileSource);


    if (file.open(QIODevice::ReadWrite)) {

        QXmlStreamReader reader(&file);
        if (reader.readNextStartElement()) {
                if (reader.name() == "CodeData"){
                    while(reader.readNextStartElement()){
                        if(reader.name() == "code"){
                            QString t = reader.attributes().value("title").toString();
                            QString s = reader.readElementText();

                            if(page < maxPage) {
                                typeLine[page].append(t.toInt());
                                textLine[page].append(s);
                                linePage[page]++;
                            }
                        }
                        else
                            reader.skipCurrentElement();
                    }
                }
                else
                    reader.raiseError(QObject::tr("Incorrect file"));
            }
    }
}

