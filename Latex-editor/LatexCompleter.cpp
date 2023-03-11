#include "LatexCompleter.h"

#include <QFile>
#include <QStringListModel>
#include <assert.h>
#include <QDebug>

LatexCompleter::LatexCompleter(QObject *parent)
    : QCompleter{parent}
{
    Q_INIT_RESOURCE(codeeditor_resources);
    QFile file(":/languages/latex_all_commands.txt");

    if (!file.open(QIODevice::ReadOnly)){
        return;
    }

    QStringList list;
    while (!file.atEnd()){
        list.append(file.readLine());
        list.back().resize(list.back().size() - 1); 
        //list.back().replace(0, 2, '/');
        list.back().remove(0, 2);
    }


    qDebug() << list << '\n'; 

    setModel(new QStringListModel(list, this));
    setCompletionColumn(0);
    setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    setCaseSensitivity(Qt::CaseSensitive);
    setWrapAround(true);
}
