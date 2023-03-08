#include "CodeEditor.h"
#include "LineNumberArea.h"

// TODO ADD MISSING INCLUDES ASPETIALY FOR MY CLASSES

#include <QFontDatabase>
#include <QList>
#include <QMimeData>
#include <QRect>
#include <QSignalBlocker>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>

CodeEditor::CodeEditor(QWidget *parent)
    : QTextEdit(parent),

      // m_highlighter(nullptr),
      // m_syntaxStyle(nullptr),
      m_lineNumberArea(new LineNumberArea(this)),
      // m_completer(nullptr),
      // m_framedAttribute(new QFramedTextAttribute(this)),

      m_auto_indentation(true), m_auto_parentheses(true), m_replace_tab(true),
      m_tab_replace(QString(4, ' ')) {
  initDocumentLayoutHandlers();
  initFont();
  performConnections();

  // setSyntaxStyle(SyntaxStyle::defaultStyle());
}

void CodeEditor::initDocumentLayoutHandlers() {
  // TODO

  // document()
  //     ->documentLayout()
  //     ->registerHandler(
  //         QFramedTextAttribute::type(),
  //         m_framedAttribute
  //     );
}

void CodeEditor::initFont() {
  auto font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  font.setFixedPitch(true);
  font.setPointSize(10);

  setFont(font);
}

void CodeEditor::performConnections() {
  /* connect(document(), &QTextDocument::blockCountChanged, this,
          &CodeEditor::updateLineNumberAreaWidth); */

  // TODO deal with document in TEXT_EDIT

  /* connect(
      verticalScrollBar(),
      &QScrollBar::valueChanged,
      [this](int){ m_line_number_area->update(); }
  ); */

  // TODO deal with verticalScrolBar();

  connect(this, &QTextEdit::cursorPositionChanged, this,
          &CodeEditor::updateExtraSelection);

  connect(this, &QTextEdit::selectionChanged, this,
          &CodeEditor::onSelectionChanged);
}

void CodeEditor::setSyntaxHighlighter(StyleSyntaxHighlighter *higlighter) {
  // TODO
}

void CodeEditor::setSyntaxStyle(SyntaxStyle *style) {
  // TODO
}

void CodeEditor::updateStyle() {
  // TODO
}

void CodeEditor::updateExtraSelection() {
  QList<QTextEdit::ExtraSelection> extra;

  highlightCurrentLine(extra);
  highlightParenthesis(extra);

  setExtraSelections(extra);
}

void CodeEditor::onSelectionChanged() {
  auto selected = textCursor().selectedText();

  auto cursor = textCursor();

  // Cursor is null if setPlainText was called. UNDERSTAND THIS
  if (cursor.isNull()) {
    return;
  }

  // Move left and select word?
  cursor.movePosition(QTextCursor::MoveOperation::Left);
  cursor.select(QTextCursor::SelectionType::WordUnderCursor);

  // block signals
  QSignalBlocker blocker(this);
  // m_framedAttribute->clear(cursor); DEAL WITH IT

  // check if we stay on selected text?
  if (selected.size() > 1 && cursor.selectedText() == selected) {
    auto backup = textCursor();

    // Perform search selecting UNDERSTAND
    // handleSelectionQuery(cursor);

    // return cursor on firs position
    setTextCursor(backup);
  }
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
  QTextEdit::resizeEvent(e);
  updateLineGeometry();
}

void CodeEditor::updateLineGeometry() {
  /* QRect cr = contentsRect();
  m_lineNumberArea->setGeometry(
      QRect(cr.left(),
            cr.top(),
            m_lineNumberArea->sizeHint().width(),
            cr.height()
      )
  ); UDATE LINE GEOMETRY BUT HOW? */
}

void CodeEditor::updateLineNumberAreaWidth(int) {
  // setViewportMargins(m_lineNumberArea->sizeHint().width(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect) {
  /* m_lineNumberArea->update(
      0,
      rect.y(),
      m_lineNumberArea->sizeHint().width(),
      rect.height()
  );
  updateLineGeometry();

  if (rect.contains(viewport()->rect()))
  {
      updateLineNumberAreaWidth(0);
  } */
}

void CodeEditor::handleSelectionQuery(QTextCursor cursor) {

  /*auto searchIterator = cursor;
  searchIterator.movePosition(QTextCursor::Start);
  searchIterator = document()->find(cursor.selectedText(), searchIterator);
  while (searchIterator.hasSelection())
  {
      m_framedAttribute->frame(searchIterator);

      searchIterator = document()->find(cursor.selectedText(), searchIterator);
  } REALIZE FRAMEDATTRIBUTE TO UNDERSTAND FULL CODE*/
}

void CodeEditor::highlightParenthesis(
    QList<QTextEdit::ExtraSelection> &extraSelection) {
  // TODO
}

void CodeEditor::highlightCurrentLine(
    QList<QTextEdit::ExtraSelection> &extraSelection) {
  // TODO
}

void CodeEditor::paintEvent(QPaintEvent *e) {
  // updateLineNumberArea(e->rect()); DO THIS
  QTextEdit::paintEvent(e);
}

int CodeEditor::getFirstVisibleBlock() {
  // TODO SOME GEOMETRY MAGICK
  return 0;
}

bool CodeEditor::proceedCompleterBegin(QKeyEvent *e) {
  // TODO SOMECOMPLETER STUFF
  return false;
}

void CodeEditor::proceedCompleterEnd(QKeyEvent *e) {
  // TODO SOME COMPLETER STUFF
}

void CodeEditor::keyPressEvent(QKeyEvent *e) {
  // TODO TOO MUCH COMPLETER STUFF
}

void CodeEditor::setAutoIndentation(bool enabled) {
  m_auto_indentation = enabled;
}

bool CodeEditor::autoIndentation() const { return m_auto_indentation; }

void CodeEditor::setAutoParentheses(bool enabled) {
  m_auto_parentheses = enabled;
}

bool CodeEditor::isAutoParentheses() const { return m_auto_parentheses; }

void CodeEditor::setTabReplace(bool enabled) { m_replace_tab = enabled; }

bool CodeEditor::tabReplace() const { return m_replace_tab; }

void CodeEditor::setTabReplaceSize(int val) {
  m_tab_replace.clear();

  m_tab_replace.fill(' ', val);
}

int CodeEditor::tabReplaceSize() const { return m_tab_replace.size(); }

void setCompleter(Completer *completer) {
  // TODO
}

void CodeEditor::focusInEvent(QFocusEvent *e) {
  // TODO COMPLETER
}

void CodeEditor::insertCompletion(QString s) {
  // TODO COMPLETER
}

Completer *CodeEditor::completer() const { return m_completer; }

QChar CodeEditor::charUnderCursor(int offset) const {
  auto block = textCursor().blockNumber();
  auto index = textCursor().positionInBlock();
  auto text = document()->findBlockByNumber(block).text();

  index += offset;

  if (index < 0 || index >= text.size()) {
    return {};
  }

  return text[index];
}

QString CodeEditor::wordUnderCursor() const {
  auto tc = textCursor();
  tc.select(QTextCursor::WordUnderCursor);
  return tc.selectedText();
}

// DEAL WITH TWO UPPER FUNCTION

void CodeEditor::insertFromMimeData(const QMimeData *source) {
  insertPlainText(source->text());
} // LIKE BUFFER(ctrl+v)?

int CodeEditor::getIndentationSpaces() {
  // TODO
  return 0;
}
