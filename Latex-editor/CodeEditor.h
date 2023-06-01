#ifndef CODEEDITOR_H
#define CODEEDITOR_H
// NOLINTNEXTLINE [clang-diagnostic-error]
#include <QCompleter>
#include <QObject>
#include <QTextEdit>

class LineNumberArea;
class SyntaxStyle;
class StyleSyntaxHighlighter; // TODO
class FramedTextAttribute;    // SEARCH TODO

class CodeEditor : public QTextEdit {
  Q_OBJECT
public:
  // just constructor
  explicit CodeEditor(QWidget *parent = nullptr);

  // disable copy
  CodeEditor(const CodeEditor &) = delete;
  CodeEditor operator=(const CodeEditor) = delete;

  // return index on first visible block
  int getFirstVisibleBlock();

  void setSyntaxStyle(SyntaxStyle *style);

  void setSyntaxHighlighter(StyleSyntaxHighlighter *higlighter);

  void setAutoParentheses(bool enabled = true);

  // get is auto parentheses enabled
  bool isAutoParentheses() const;

  void setTabReplace(bool enabled = true);

  bool tabReplace() const;

  // get is tab replace enabled
  bool isTabReplace() const;

  void setTabReplaceSize(int value = 4);

  // get size of tab, default value equal to 4
  int tabReplaceSize() const;

  void setAutoIndentation(bool enabled = true);

  bool autoIndentation() const;

  void setCompleter(QCompleter *completer);

  QCompleter *completer() const;

public
  Q_SLOT :

      // Slot that performs insertion of comletion
      void
      insertCompletion(QString s);

  // Slot that that performs update LineNumberArea Width
  void updateLineNumberAreaWidth(int);

  // Slot that performs update of some part of line number area
  void updateLineNumberArea(const QRect &rect);

  // Slot that will proceed extra selection for current cursor position.
  void updateExtraSelection();

  // Slot that will update editor style.
  void updateStyle();

  // Slot that will be called on selection change.
  void onSelectionChanged();

protected:
  // overrided methods

  // insert text as plain text
  void insertFromMimeData(const QMimeData *source) override;

  // called on editor painting, override for add line number area
  void paintEvent(QPaintEvent *e) override;

  // called on widget resize, override for add line number area
  void resizeEvent(QResizeEvent *e) override;

  /*called on any keypress, overload for
    1. Completion
    2. Tab to space
    3. Low identation
    4. Auto parenthesis
*/
  void keyPressEvent(QKeyEvent *e) override;

  // called on focus in widget, override for set comleter
  void focusInEvent(QFocusEvent *e) override;

private:
  // Method for initializing document layout
  void initDocumentLayoutHandlers();

  // Method for initializing default monospace font.
  void initFont();

  // Method for performing connection of objects.
  void performConnections();

  // Method, that performs selection frame selection.
  void handleSelectionQuery(QTextCursor cursor);

  // Method for updating geometry of line number area.
  void updateLineGeometry();

  /*  Method that performs Completer processing.
    Returns true if event has to be dropped.
    return Shall event be dropped.
*/
  bool proceedCompleterBegin(QKeyEvent *e);
  void proceedCompleterEnd(QKeyEvent *e);

  // Method for getting character under cursor.
  QChar charUnderCursor(int offset = 0) const;

  // Method for getting word under cursor.
  QString wordUnderCursor() const;

  // Method that adds highlighting of currently selected line to extra selection
  // list.
  void highlightCurrentLine(QList<QTextEdit::ExtraSelection> &extraSelection);

  // Method, that adds highlighting of parenthesis if available.
  void highlightParenthesis(QList<QTextEdit::ExtraSelection> &extraSelection);

  // Method for getting number of indentation spaces in current line. Tabs will
  // be treated as `tabWidth / spaceWidth`
  int getIndentationSpaces();

  StyleSyntaxHighlighter *m_highlighter;
  SyntaxStyle *m_syntax_style;
  LineNumberArea *m_line_number_area;
  QCompleter *m_completer;

  FramedTextAttribute *m_framedAttribute;

  bool m_auto_indentation;
  bool m_auto_parentheses;
  bool m_replace_tab;
  QString m_tab_replace;
};

#endif // CODEEDITOR_H
