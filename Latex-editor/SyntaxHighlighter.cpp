#include "SyntaxHighlighter.h"

#include <QTextDocument>
#include <utility>

const int BOLD_FONT = 75;
const int NORMAL_FONT = 50;
const QColor BLUE = QColor(28, 28, 252);
const QColor VIOLET = QColor(152, 89, 196);
const QColor LIGHT_GREEN = QColor(77, 150, 80);
const QColor LIGHT_BLUE = QColor(124, 168, 147);
const QColor MAGENTA = QColor(133, 106, 142);

void SyntaxHighlighter::addRule(QTextCharFormat *format,
                                const QColor &foreground, int font,
                                QRegularExpression pattern) {
    format->setForeground(foreground);
    format->setFontWeight(font);
    rule.pattern = std::move(pattern);
    rule.format = *format;
    highlighting_rules.append(rule);
}

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent) {
    QColor function_color = BLUE;
    addRule(&command_format, function_color, BOLD_FONT,
            QRegularExpression(QStringLiteral("\\\\[A-Za-z]+")));

    QColor escaping_color = VIOLET;
    addRule(&escaping_format, escaping_color, NORMAL_FONT,
            QRegularExpression(QStringLiteral("\\\\[^A-Za-z]{1}")));

    QColor math_color = LIGHT_GREEN;
    addRule(&math_format, math_color, NORMAL_FONT,
            QRegularExpression(QStringLiteral("\\$.*\\$")));

    QColor comment_color = LIGHT_BLUE;
    addRule(&comment_format, comment_color, NORMAL_FONT,
            QRegularExpression(QStringLiteral("%[^\\n]*")));

    QColor options_color = MAGENTA;
    options = {
            OptionPatterns(QRegularExpression(QStringLiteral("\\\\begin{")), 7),
            OptionPatterns(QRegularExpression(QStringLiteral("\\\\end{")), 5),
            OptionPatterns(QRegularExpression(QStringLiteral("\\\\usepackage{")),
                           12),
            OptionPatterns(QRegularExpression(QStringLiteral("\\\\documentclass{")),
                           15),
    };
    options_format.setForeground(options_color);
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule: highlighting_rules) {
        QRegularExpressionMatchIterator matchIterator =
                rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(),
                      rule.format);
        }
    }

    for (auto &option: options) {
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(option.begin);

        while (startIndex >= 0) {
            QRegularExpressionMatch match = option.end.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex + match.capturedLength();
            }
            setFormat(startIndex + option.length,
                      commentLength - (option.length + 1), options_format);
            startIndex = text.indexOf(option.begin, startIndex + commentLength);
        }
    }
}

SyntaxHighlighter::OptionPatterns::OptionPatterns(
        const QRegularExpression &begin_, int length_)
        : begin(begin_), end(QRegularExpression(QStringLiteral("}"))),
          length(length_) {}