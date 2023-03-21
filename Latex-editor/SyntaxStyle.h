#ifndef SYNTAXSTYLE_H
#define SYNTAXSTYLE_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QTextCharFormat>

class SyntaxStyle : public QObject {
  Q_OBJECT
public:
  // Constructor
  explicit SyntaxStyle(QObject *parent = nullptr);

  // Method for load style
  bool load(QString file_name);

  // Method for getting style name
  QString name() const;

  // Method for check is any syle loaded
  bool isLoaded() const;

  // Method to geting format by name
  QTextCharFormat getFormat(QString name) const;

  static SyntaxStyle *defaultStyle();

private:
  QString m_name;

  QMap<QString, QTextCharFormat> m_data;

  bool m_loaded;
};

#endif // SYNTAXSTYLE_H
