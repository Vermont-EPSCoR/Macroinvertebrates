#include "invertebratehandler.h"

InvertebrateHandler::InvertebrateHandler()
{
    curlyBraceElement.setPattern("{{(.+?)}}");
    curlyBraceElement.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);

    textBlock.setPattern("\|text\s*=\s*(.+?)<!--Stop-->");
    textBlock.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
}

void InvertebrateHandler::parseInfoboxToInvertebrate(const QString &infoBox, Invertebrate &invertebrate)
{
    QRegularExpressionMatch match = textBlock.match(infoBox);
    if(match.hasMatch()) {
        invertebrate.description = match.captured(1);
    } else {
        qDebug() << "Something went wrong with the match";
    }

    for(QString line: infoBox.split("\n", QString::SkipEmptyParts)) {
        QStringList pair = line.split(QRegularExpression(" ?= ?"));
        if(pair.length() != 2) {
            qDebug() << "Pair does not contain 2 elements" << pair;
        } else {
            QString key = pair.at(0).trimmed();
            QString value = pair.at(1).trimmed();

            if(key == "|name") {
                qDebug() << "name";
                invertebrate.name = value;
            } else if(key == "|common name") {
                qDebug() << "common name";
                invertebrate.commonName = value;
            } else if(key == "|family") {
                qDebug() << "family";
                invertebrate.family = value;
            } else if(key == "|fly name") {
                qDebug() << "fly name";
                invertebrate.flyName = value;
            } else if(key == "|genus") {
                qDebug() << "genus";
                invertebrate.genus = value;
            } else if(key == "|image") {
                qDebug() << "image";
                invertebrate.imageFileRemote = value;  // write method to get url for this image
//                invertebrate.imageFileLocal = ;  // TODO
            } else if(key == "|order") {
                invertebrate.order = value;
            } else if(key != "|text") {
                qDebug() << "Key: " << key.trimmed() << " Value: " << value.trimmed();
            }
        }
    }
}

Invertebrate InvertebrateHandler::parse(const QString &text)
{
    Invertebrate invertebrate;

    QRegularExpressionMatchIterator iter = curlyBraceElement.globalMatch(text);
    while(iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        QString matchingString = match.captured(1);

        parseInfoboxToInvertebrate(matchingString, invertebrate);
    }

    return invertebrate;
}

InvertebrateHandler::~InvertebrateHandler()
{}
