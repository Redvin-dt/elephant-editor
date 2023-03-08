#include "LineNumberArea.h"
#include "CodeEditor.h"
//TODO ADD MISSING INCLUDE

#include <QTextEdit>
#include <QPaintEvent>

LineNumberArea::LineNumberArea(CodeEditor *parent) : 
    QWidget(parent),
    syntax_style(nullptr),
    code_editor(parent) {}


QSize LineNumberArea::sizeHint() const {
    //TODO SOME CALCULATION WITH WIDTH
}

void LineNumberArea::setSyntaxStyle(SyntaxStyle *style){
    syntax_style = style;
}

SyntaxStyle *LineNumberArea::syntaxStyle() const {
    return syntax_style;
}

void LineNumberArea::paintEvent(QPaintEvent *event){
    //TODO READ MORE ABOUT PAINT EVENT
}