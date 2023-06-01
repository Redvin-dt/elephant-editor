#include "SyntaxStyle.h"

#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QTextCharFormat>
#include <QXmlStreamReader>

SyntaxStyle::SyntaxStyle(QObject *parent)
    : QObject{parent}, m_name(), m_data(), m_loaded(false) {}

bool SyntaxStyle::load(QString file_name) {
    QXmlStreamReader reader(file_name);

    while (!reader.atEnd() && !reader.hasError()) {
        auto item = reader.readNext();

        if (item == QXmlStreamReader::StartElement) {
            if (reader.name() == "style-scheme") {
                if (reader.attributes().hasAttribute("name")) {
                    m_name = reader.attributes().value("name").toString();
                }

            } else if (reader.name() == "style") {
                auto attributes = reader.attributes();

                auto name = attributes.value("name");

                // Set new format
                QTextCharFormat format;

                if (attributes.hasAttribute("background")) {
                    format.setBackground(
                        QColor(attributes.value("background").toString()));
                }

                if (attributes.hasAttribute("foreground")) {
                    format.setForeground(
                        QColor(attributes.value("foreground").toString()));
                }

                if (attributes.hasAttribute("bold") &&
                    attributes.value("bold") == "true") {
                    format.setFontWeight(QFont::Weight::Bold);
                }

                if (attributes.hasAttribute("italic") &&
                    attributes.value("italic") == "true") {
                    format.setFontItalic(true);
                }

                if (attributes.hasAttribute("underlineStyle")) {
                    auto underline = attributes.value("underlineStyle");

                    auto s = QTextCharFormat::UnderlineStyle::NoUnderline;

                    if (underline == "SingleUnderline") {
                        s = QTextCharFormat::UnderlineStyle::SingleUnderline;
                    } else if (underline == "DashUnderline") {
                        s = QTextCharFormat::UnderlineStyle::DashUnderline;
                    } else if (underline == "DotLine") {
                        s = QTextCharFormat::UnderlineStyle::DotLine;
                    } else if (underline == "DashDotLine") {
                        s = QTextCharFormat::DashDotLine;
                    } else if (underline == "DashDotDotLine") {
                        s = QTextCharFormat::DashDotDotLine;
                    } else if (underline == "WaveUnderline") {
                        s = QTextCharFormat::WaveUnderline;
                    } else if (underline == "SpellCheckUnderline") {
                        s = QTextCharFormat::SpellCheckUnderline;
                    } else {
                        qDebug() << "Unknown underline value " << underline;
                    }

                    format.setUnderlineStyle(s);
                }

                m_data[name.toString()] = format;
            }
        }
    }

    m_loaded = !reader.hasError();
    return m_loaded;
}

QString SyntaxStyle::name() const { return m_name; }

QTextCharFormat SyntaxStyle::getFormat(QString name) const {
    if (m_data.find(name) != m_data.end()) {
        return m_data[name];
    } else {
        return QTextCharFormat();
    }
}

bool SyntaxStyle::isLoaded() const { return m_loaded; }

SyntaxStyle *SyntaxStyle::defaultStyle() {
    static SyntaxStyle default_style;

    if (!default_style.isLoaded()) {
        Q_INIT_RESOURCE(codeeditor_resources);
        QFile file(":/default_style.xml");

        if (!file.open(QIODevice::ReadOnly)) {
            return &default_style;
        }

        if (!default_style.load(file.readAll())) {
            qDebug() << "Can't load this style";
        }
    }

    return &default_style;
}
