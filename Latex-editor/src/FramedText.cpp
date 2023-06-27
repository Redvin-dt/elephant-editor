#include "../include/FramedText.h"
#include "../include/SyntaxStyle.h"
#include <QDebug>
#include <QFontMetrics>
#include <QPainter>
#include <QTextBlock>

int FramedText::type() { return QTextFormat::UserFormat + 1; }

FramedText::FramedText(QObject *parent) : QObject(parent), m_style(nullptr) {}

void FramedText::setSyntaxStyle(SyntaxStyle *style) { m_style = style; }

SyntaxStyle *FramedText::syntaxStyle() const { return m_style; }

QSizeF FramedText::intrinsicSize(QTextDocument *, int, const QTextFormat &) {
    return {0, 0};
}

void FramedText::drawObject(QPainter *painter, const QRectF &rect,
                            QTextDocument *, int, const QTextFormat &format) {
    // Casting
    auto textCharFormat = reinterpret_cast<const QTextCharFormat &>(format);

    // Getting font data
    auto font = textCharFormat.font();
    QFontMetrics metrics(font);

    // Getting required size
    auto string = format.property(FramedString).toString();
    auto stringSize = metrics.boundingRect(string).size();

    // Creating frame rect
    QRectF drawRect(rect.topLeft(), stringSize);
    drawRect.moveTop(rect.top() - stringSize.height());
    drawRect.adjust(0, 4, 0, 4);

    // Drawing
    painter->setPen(m_style->getFormat("Occurrences").background().color());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(drawRect, 4, 4);
}

void FramedText::frame(QTextCursor cursor) {
    auto text =
            cursor.document()->findBlockByNumber(cursor.blockNumber()).text();

    QTextCharFormat format;
    format.setObjectType(type());
    format.setProperty(FramedString, cursor.selectedText());

    if (cursor.selectionEnd() > cursor.selectionStart()) {
        cursor.setPosition(cursor.selectionStart());
    } else {
        cursor.setPosition(cursor.selectionEnd());
    }

    cursor.insertText(QString(QChar::ObjectReplacementCharacter), format);
}

void FramedText::clear(QTextCursor cursor) {
    auto doc = cursor.document();

    for (auto blockIndex = 0; blockIndex < doc->blockCount(); ++blockIndex) {
        auto block = doc->findBlockByNumber(blockIndex);

        auto formats = block.textFormats();
        int offset = 0;

        for (auto &format: formats) {
            if (format.format.objectType() == type()) {
                cursor.setPosition(block.position() + format.start - offset);
                cursor.deleteChar();
                ++offset;
            }
        }
    }
}