#ifndef LATEXCOMPLETER_H
#define LATEXCOMPLETER_H

#include <QCompleter>
#include <QObject>

class LatexCompleter : public QCompleter {
Q_OBJECT
public:
    explicit LatexCompleter(QObject *parent = nullptr);
};

#endif // LATEXCOMPLETER_H
