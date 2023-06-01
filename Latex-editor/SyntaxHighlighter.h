#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class QTextDocument;

class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
  public:
    explicit SyntaxHighlighter(QTextDocument *parent = nullptr);
    ~SyntaxHighlighter() override {}

  protected:
    void highlightBlock(const QString &text) override;

  private:
    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    struct OptionPatterns {
        OptionPatterns(const QRegularExpression &begin_, int length_);

        QRegularExpression begin;
        QRegularExpression end;
        int length;
    };
    QVector<OptionPatterns> options;
    QTextCharFormat options_format;

    QVector<HighlightingRule> highlighting_rules;
    HighlightingRule rule;

    QRegularExpression begin_start_expression;
    QRegularExpression begin_end_expression;

    QTextCharFormat command_format;
    QTextCharFormat math_format;
    QTextCharFormat escaping_format;
    QTextCharFormat comment_format;

    void addRule(QTextCharFormat *format, const QColor &foreground, int font,
                 QRegularExpression pattern);
};

#endif // SYNTAXHIGHLIGHTER_H