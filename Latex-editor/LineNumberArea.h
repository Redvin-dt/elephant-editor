#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include "SyntaxStyle.h"

#include <QObject>
#include <QWidget>

class CodeEditor;

class LineNumberArea : public QWidget {
  Q_OBJECT
public:
  // set code editor as parent
  explicit LineNumberArea(CodeEditor *parent = nullptr);

  // Disable copying
  LineNumberArea(const LineNumberArea &) = delete;
  LineNumberArea &operator=(const LineNumberArea &) = delete;

  QSize sizeHint() const override;

  void setSyntaxStyle(SyntaxStyle *style);

  SyntaxStyle *syntaxStyle() const;

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  CodeEditor *m_code_editor;
  SyntaxStyle *m_syntax_style;
};

#endif // LINENUMBERAREA_H
