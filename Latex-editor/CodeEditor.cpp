#include "CodeEditor.h"
#include "LatexCompleter.h"
#include "LineNumberArea.h"
#include "SyntaxStyle.h"

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QCompleter>
#include <QDebug>
#include <QFontDatabase>
#include <QList>
#include <QMimeData>
#include <QPalette>
#include <QRect>
#include <QScrollBar>
#include <QShortcut>
#include <QSignalBlocker>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>
#include <iostream>

static QVector<QPair<QString, QString>> parentheses = {
    {"(", ")"}, {"{", "}"}, {"[", "]"}, {"\"", "\""}, {"'", "'"}};

CodeEditor::CodeEditor(QWidget *parent)
    : QTextEdit(parent), m_syntax_style(nullptr),
      m_line_number_area(new LineNumberArea(this)), m_completer(nullptr),
      m_framed_attribute(new FramedText(this)),

      m_auto_indentation(true), m_auto_parentheses(true), m_replace_tab(true),
      m_tab_replace(QString(4, ' ')) {
    initDocumentLayoutHandlers();
    initFont();
    performConnections();
    setCompleter(new LatexCompleter(this));

    setSyntaxStyle(SyntaxStyle::defaultStyle());
}

void CodeEditor::initDocumentLayoutHandlers() {
    document()->documentLayout()->registerHandler(FramedText::type(),
                                                  m_framed_attribute);
}

void CodeEditor::initFont() {
    auto font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setFixedPitch(true);
    font.setPointSize(10);

    setFont(font);
}

void CodeEditor::performConnections() {
    connect(document(), &QTextDocument::blockCountChanged, this,
            &CodeEditor::updateLineNumberAreaWidth);

    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            [this](int) { m_line_number_area->update(); });

    connect(this, &QTextEdit::cursorPositionChanged, this,
            &CodeEditor::updateExtraSelection);

    connect(this, &QTextEdit::selectionChanged, this,
            &CodeEditor::onSelectionChanged);
}

void CodeEditor::setSyntaxStyle(SyntaxStyle *style) {
    m_syntax_style = style;

    m_framed_attribute->setSyntaxStyle(m_syntax_style);
    m_line_number_area->setSyntaxStyle(m_syntax_style);

    updateStyle();
}

void CodeEditor::updateStyle() {

    if (m_syntax_style) {
        auto currentPalette = palette();

        // Setting text format/color
        currentPalette.setColor(
            QPalette::ColorRole::Text,
            m_syntax_style->getFormat("Text").foreground().color());

        // Setting common background
        currentPalette.setColor(
            QPalette::Base,
            m_syntax_style->getFormat("Text").background().color());

        // Setting selection color
        currentPalette.setColor(
            QPalette::Highlight,
            m_syntax_style->getFormat("Selection").background().color());

        setPalette(currentPalette);
    }

    updateExtraSelection();
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

    // Cursor is null if setPlainText was called.
    if (cursor.isNull()) {
        return;
    }

    cursor.movePosition(QTextCursor::MoveOperation::Left);
    cursor.select(QTextCursor::SelectionType::WordUnderCursor);

    // block signals
    QSignalBlocker blocker(this);
    m_framed_attribute->clear(cursor);

    // check if we stay on selected text?
    if (selected.size() > 1 && cursor.selectedText() == selected) {
        auto backup = textCursor();

        // Perform search selecting
        handleSelectionQuery(cursor);

        // return cursor on firs position
        setTextCursor(backup);
    }
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QTextEdit::resizeEvent(e);
    updateLineGeometry();
}

void CodeEditor::updateLineGeometry() {
    QRect cr = contentsRect();
    m_line_number_area->setGeometry(
        QRect(cr.left(), cr.top(), m_line_number_area->sizeHint().width(),
              cr.height()));
}

void CodeEditor::updateLineNumberAreaWidth(int) {
    setViewportMargins(m_line_number_area->sizeHint().width(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect) {
    m_line_number_area->update(
        0, rect.y(), m_line_number_area->sizeHint().width(), rect.height());
    updateLineGeometry();

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void CodeEditor::handleSelectionQuery([[maybe_unused]] QTextCursor cursor) {
    auto searchIterator = cursor;
    searchIterator.movePosition(QTextCursor::Start);
    searchIterator = document()->find(cursor.selectedText(), searchIterator);
    while (searchIterator.hasSelection()) {
        m_framed_attribute->frame(searchIterator);
        searchIterator =
            document()->find(cursor.selectedText(), searchIterator);
    }
}

void CodeEditor::highlightParenthesis(
    QList<QTextEdit::ExtraSelection> &extraSelection) {
    auto currentSymbol = charUnderCursor();
    auto prevSymbol = charUnderCursor(-1);

    for (auto &pair : parentheses) {
        int direction;

        QChar counterSymbol;
        QChar activeSymbol;
        auto position = textCursor().position();

        if (pair.first == currentSymbol) {
            direction = 1;
            counterSymbol = pair.second[0];
            activeSymbol = currentSymbol;
        } else if (pair.second == prevSymbol) {
            direction = -1;
            counterSymbol = pair.first[0];
            activeSymbol = prevSymbol;
            position--;
        } else {
            continue;
        }

        auto counter = 1;

        while (counter != 0 && position > 0 &&
               position < (document()->characterCount() - 1)) {
            // Moving position
            position += direction;

            auto character = document()->characterAt(position);
            // Checking symbol under position
            if (character == activeSymbol) {
                ++counter;
            } else if (character == counterSymbol) {
                --counter;
            }
        }

        auto format = m_syntax_style->getFormat("Parentheses");

        // Found
        if (counter == 0) {
            ExtraSelection selection{};

            auto directionEnum = direction < 0
                                     ? QTextCursor::MoveOperation::Left
                                     : QTextCursor::MoveOperation::Right;

            selection.format = format;
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            selection.cursor.movePosition(
                directionEnum, QTextCursor::MoveMode::MoveAnchor,
                std::abs(textCursor().position() - position));

            selection.cursor.movePosition(QTextCursor::MoveOperation::Right,
                                          QTextCursor::MoveMode::KeepAnchor, 1);

            extraSelection.append(selection);

            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            selection.cursor.movePosition(directionEnum,
                                          QTextCursor::MoveMode::KeepAnchor, 1);

            extraSelection.append(selection);
        }

        break;
    }
}

void CodeEditor::highlightCurrentLine(
    QList<QTextEdit::ExtraSelection> &extraSelection) {
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection{};

        selection.format = m_syntax_style->getFormat("CurrentLine");
        selection.format.setForeground(QBrush());
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();

        extraSelection.append(selection);
    }
}

void CodeEditor::paintEvent(QPaintEvent *e) {
    updateLineNumberArea(e->rect());
    QTextEdit::paintEvent(e);
}

int CodeEditor::getFirstVisibleBlock() {
    QTextCursor cursor = QTextCursor(document());

    for (int index = 0; index < document()->blockCount(); index++) {
        QTextBlock block = cursor.block();

        QRect first = viewport()->geometry();
        QRect second =
            document()
                ->documentLayout()
                ->blockBoundingRect(block)
                .translated(viewport()->geometry().x(),
                            viewport()->geometry().y() -
                                verticalScrollBar()->sliderPosition())
                .toRect();

        if (first.intersects(second)) {
            return index;
        }

        cursor.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

bool CodeEditor::proceedCompleterBegin(QKeyEvent *e) {
    if (m_completer && m_completer->popup()->isVisible()) {
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return true; // let the completer do default behavior
        default:
            break;
        }
    }

    auto is_shortcut =
        ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);

    return m_completer && is_shortcut;
}

void CodeEditor::proceedCompleterEnd(QKeyEvent *e) {
    auto ctrl_or_shift =
        e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

    if (!m_completer || (ctrl_or_shift && e->text().isEmpty()) ||
        e->key() == Qt::Key_Delete) {
        return;
    }

    static QString special_symbols(R"(~!@#$%^&*()_+{}|:"<>?,./;'[]\-=)");

    auto isShortcut =
        ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);
    auto prefix = wordUnderCursor();

    if (!isShortcut && (e->text().isEmpty() || prefix.length() < 2 ||
                        special_symbols.contains(e->text().right(1)))) {
        m_completer->popup()->hide();
        return;
    }

    if (prefix != m_completer->completionPrefix()) {
        m_completer->setCompletionPrefix(prefix);
        m_completer->popup()->setCurrentIndex(
            m_completer->completionModel()->index(0, 0));
    }

    auto cursor_rect = cursorRect();
    cursor_rect.setWidth(
        m_completer->popup()->sizeHintForColumn(0) +
        m_completer->popup()->verticalScrollBar()->sizeHint().width());

    m_completer->complete(cursor_rect);
}

void CodeEditor::keyPressEvent(QKeyEvent *e) {
    const int defaultIndent =
        tabStopDistance() / fontMetrics().averageCharWidth();

    auto completer_skip = proceedCompleterBegin(e);

    if (!completer_skip) {
        if (m_replace_tab && e->key() == Qt::Key_Tab &&
            e->modifiers() == Qt::NoModifier) {
            insertPlainText(m_tab_replace);
        }
    }

    int indentation_level = getIndentationSpaces();

    int tab_count = indentation_level * fontMetrics().averageCharWidth() /
                    tabStopDistance();

    // Have Qt Edior like behaviour, if {|} and enter is pressed indent the two
    // parenthesis
    if (m_auto_indentation &&
        (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) &&
        charUnderCursor() == '}' && charUnderCursor(-1) == '{') {
        int charsBack = 0;
        insertPlainText("\n");

        if (m_replace_tab)
            insertPlainText(QString(indentation_level + defaultIndent, ' '));
        else
            insertPlainText(QString(tab_count + 1, '\t'));

        insertPlainText("\n");
        charsBack++;

        if (m_replace_tab) {
            insertPlainText(QString(indentation_level, ' '));
            charsBack += indentation_level;
        } else {
            insertPlainText(QString(tab_count, '\t'));
            charsBack += tab_count;
        }

        while (charsBack--)
            moveCursor(QTextCursor::MoveOperation::Left);
        return;
    }

    // Shortcut for moving line to left
    if (m_replace_tab && e->key() == Qt::Key_Backtab) {
        indentation_level = std::min(indentation_level, m_tab_replace.size());

        auto cursor = textCursor();

        cursor.movePosition(QTextCursor::MoveOperation::StartOfLine);
        cursor.movePosition(QTextCursor::MoveOperation::Right,
                            QTextCursor::MoveMode::KeepAnchor,
                            indentation_level);

        cursor.removeSelectedText();
        return;
    }

    QTextEdit::keyPressEvent(e);

    if (m_auto_indentation &&
        (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)) {
        if (m_replace_tab)
            insertPlainText(QString(indentation_level, ' '));
        else
            insertPlainText(QString(tab_count, '\t'));
    }

    if (m_auto_parentheses) {
        for (auto &&el : parentheses) {
            // Inserting closed brace
            if (el.first == e->text()) {
                insertPlainText(el.second);
                moveCursor(QTextCursor::MoveOperation::Left);
                break;
            }

            // If it's close brace - check parentheses
            if (el.second == e->text()) {
                auto symbol = charUnderCursor();

                if (symbol == el.second) {
                    textCursor().deletePreviousChar();
                    moveCursor(QTextCursor::MoveOperation::Right);
                }

                break;
            }
        }
    }

    proceedCompleterEnd(e);
}

[[maybe_unused]] void CodeEditor::setAutoIndentation(bool enabled) {
    m_auto_indentation = enabled;
}

[[maybe_unused]] bool CodeEditor::autoIndentation() const {
    return m_auto_indentation;
}

[[maybe_unused]] void CodeEditor::setAutoParentheses(bool enabled) {
    m_auto_parentheses = enabled;
}

[[maybe_unused]] bool CodeEditor::isAutoParentheses() const {
    return m_auto_parentheses;
}

[[maybe_unused]] void CodeEditor::setTabReplace(bool enabled) {
    m_replace_tab = enabled;
}

[[maybe_unused]] bool CodeEditor::tabReplace() const { return m_replace_tab; }

[[maybe_unused]] void CodeEditor::setTabReplaceSize(int val) {
    m_tab_replace.clear();

    m_tab_replace.fill(' ', val);
}

[[maybe_unused]] int CodeEditor::tabReplaceSize() const {
    return m_tab_replace.size();
}

[[maybe_unused]] void CodeEditor::setCompleter(QCompleter *completer) {

    if (m_completer) {
        disconnect(m_completer, nullptr, this, nullptr);
    }

    m_completer = completer;

    if (!m_completer) {
        return;
    }

    m_completer->setWidget(this);
    m_completer->setCompletionMode(QCompleter::CompletionMode::PopupCompletion);

    connect(m_completer, QOverload<const QString &>::of(&QCompleter::activated),
            this, &CodeEditor::insertCompletion);
}

void CodeEditor::focusInEvent(QFocusEvent *e) {
    if (m_completer) {
        m_completer->setWidget(this);
    }

    QTextEdit::focusInEvent(e);
}

void CodeEditor::insertCompletion(QString s) {
    if (m_completer->widget() != this) {
        return;
    }

    auto tc = textCursor();
    tc.select(QTextCursor::SelectionType::WordUnderCursor);
    tc.insertText(s);
    setTextCursor(tc);
}

QCompleter *CodeEditor::completer() const { return m_completer; }

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

void CodeEditor::insertFromMimeData(const QMimeData *source) {
    insertPlainText(source->text());
}

int CodeEditor::getIndentationSpaces() {
    auto blockText = textCursor().block().text();

    int indentationLevel = 0;

    for (auto i = 0;
         i < blockText.size() && QString("\t ").contains(blockText[i]); ++i) {
        if (blockText[i] == ' ') {
            indentationLevel++;
        } else {
            indentationLevel +=
                tabStopDistance() / fontMetrics().averageCharWidth();
        }
    }

    return indentationLevel;
}
