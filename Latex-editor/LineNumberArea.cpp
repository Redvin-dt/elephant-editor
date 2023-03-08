#include "LineNumberArea.h"
#include "CodeEditor.h"
// TODO ADD MISSING INCLUDE

#include <QAbstractTextDocumentLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextEdit>

LineNumberArea::LineNumberArea(CodeEditor *parent)
    : QWidget(parent), syntax_style(nullptr), code_editor(parent) {}

QSize LineNumberArea::sizeHint() const {
  if (code_editor == nullptr) {
    return QWidget::sizeHint();
  }

  int digits = 0;
  int number = qMax(1, code_editor->document()->blockCount());

  while (number > 0) {
    digits++;
    number /= 10;
  }

  int space =
      13 +
      code_editor->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  return QSize(space, 0);
}

void LineNumberArea::setSyntaxStyle(SyntaxStyle *style) {
  syntax_style = style;
}

SyntaxStyle *LineNumberArea::syntaxStyle() const { return syntax_style; }

void LineNumberArea::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  // Paint rect with syntax style
  // TODO READ MORE ABOUT PAINT EVENT
}
