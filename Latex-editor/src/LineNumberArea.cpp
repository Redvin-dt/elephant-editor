#include "../include/LineNumberArea.h"
#include "../include/CodeEditor.h"
#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>
#include <QTextBlock>

LineNumberArea::LineNumberArea(CodeEditor *parent)
        : QWidget(parent), m_syntax_style(nullptr), m_code_editor(parent) {}

QSize LineNumberArea::sizeHint() const {
    if (m_code_editor == nullptr) {
        return QWidget::sizeHint();
    }

    int digits = 0;
    int number = qMax(1, m_code_editor->document()->blockCount());

    while (number > 0) {
        digits++;
        number /= 10;
    }

    int space =
            13 +
            m_code_editor->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;


    return QSize(space, 0);
}

void LineNumberArea::setSyntaxStyle(SyntaxStyle *style) {
    m_syntax_style = style;
}

SyntaxStyle *LineNumberArea::syntaxStyle() const { return m_syntax_style; }

void LineNumberArea::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    painter.fillRect(event->rect(),
                     m_syntax_style->getFormat("Text").background().color());

    // get possition?
    auto block_number = m_code_editor->getFirstVisibleBlock();
    auto block = m_code_editor->document()->findBlockByNumber(block_number);
    auto top = static_cast<int>(
            m_code_editor->document()
                    ->documentLayout()
                    ->blockBoundingRect(block)
                    .translated(0, -m_code_editor->verticalScrollBar()->value())
                    .top());
    auto bottom = top + static_cast<int>(m_code_editor->document()
            ->documentLayout()
            ->blockBoundingRect(block)
            .height());

    // get num color
    auto currentLine =
            m_syntax_style->getFormat("CurrentLineNumber").foreground().color();
    auto otherLines =
            m_syntax_style->getFormat("LineNumber").foreground().color();

    painter.setFont(m_code_editor->font());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(block_number + 1);

            auto isCurrentLine =
                    m_code_editor->textCursor().blockNumber() == block_number;
            painter.setPen(isCurrentLine ? currentLine : otherLines);

            painter.drawText(-5, top, sizeHint().width(),
                             m_code_editor->fontMetrics().height(), Qt::AlignRight,
                             number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(m_code_editor->document()
                ->documentLayout()
                ->blockBoundingRect(block)
                .height());
        ++block_number;
    }
}
