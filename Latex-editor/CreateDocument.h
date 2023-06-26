#ifndef CREATEDOCUMENT_H
#define CREATEDOCUMENT_H

#include <QDialog>
#include <QString>
#include <QStringList>
namespace Ui {
class CreateDocument;
}

class CreateDocument : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDocument(QString* d_name, QString* colab, QWidget *parent = nullptr);
    ~CreateDocument();

private slots:
    void on_Add_clicked();

    void on_Create_doc_clicked();

private:

    Ui::CreateDocument *ui;
    QString *doc_name = nullptr;
    QString *collaborators = nullptr;
};

#endif // CREATEDOCUMENT_H
