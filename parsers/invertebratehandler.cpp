#include "invertebratehandler.h"

InvertebrateHandler::InvertebrateHandler()
{
    curlyBraceElement.setPattern("{{InsectSection(.+?)}}");
    curlyBraceElement.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);

    textBlock.setPattern("\\|text\\s*=\\s*(.+?)<!--Stop-->");
    textBlock.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);

    textBlockWithoutStop.setPattern("\\|text\\s*=\\s*(.+?)");
    textBlockWithoutStop.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
}

void InvertebrateHandler::parseInfoboxToInvertebrate(const QString &infoBox, Invertebrate &invertebrate)
{
    QRegularExpressionMatch match = textBlock.match(infoBox);
    if(match.hasMatch()) {
        invertebrate.description = match.captured(1).trimmed();
    } else {
        match = textBlockWithoutStop.match(infoBox);
        if(match.hasMatch()) {
            invertebrate.description = match.captured(1).trimmed();
        } else {
            qDebug() << "Something went wrong with the match: " << infoBox;
        }
    }

    for(QString line: infoBox.split("\n", QString::SkipEmptyParts)) {
        QStringList pair = line.split(QRegularExpression(" ?= ?"));
        if(pair.length() != 2) {
//            qDebug() << "Pair does not contain 2 elements" << pair;
        } else {
            QString key = pair.at(0).trimmed();
            QString value = pair.at(1).trimmed();

            if(key == "|name") {
                invertebrate.name = value;
            } else if(key == "|common name") {
                invertebrate.commonName = value;
            } else if(key == "|family") {
                invertebrate.family = value;
            } else if(key == "|fly name") {
                invertebrate.flyName = value;
            } else if(key == "|genus") {
                invertebrate.genus = value;
            } else if(key == "|image") {
                invertebrate.imageFileRemote = value;
            } else if(key == "|order") {
                invertebrate.order = value;
            } else if(key != "|text") {
//                qDebug() << "Key: " << key << " Value: " << value;
            }
        }
    }

#ifndef QT_NO_DEBUG_OUTPUT
//    if(!validate(invertebrate)) {
//        qDebug() << "INFOBOX FOLLOWS" << "\n" << infoBox;
//    }
#endif
}

bool InvertebrateHandler::validate(const Invertebrate &invertebrate)
{
    QStringList nullAttributes;

//    commonName is commonly missing and may not be an error?
//    if(invertebrate.commonName.isEmpty()) {
//        nullAttributes << "commonName";
//    }

    if(invertebrate.description.isEmpty()) {
        nullAttributes << "description/title";
    }

    if(invertebrate.imageFileRemote.isEmpty()) {
        nullAttributes << "image";
    }

    if(invertebrate.family.isEmpty()) {
        nullAttributes << "family";
    }

    if(invertebrate.genus.isEmpty()) {
        nullAttributes << "genus";
    }

    if(invertebrate.name.isEmpty()) {
        nullAttributes << "name";
    }

    if(invertebrate.order.isEmpty()) {
        nullAttributes << "family";
    }

    if(nullAttributes.count() > 0) {
        qDebug() << "INVALID INVERTEBRATE: " << invertebrate.name << " IS MISSING: " << nullAttributes;
        return false;
    }

    return true;
}

Invertebrate InvertebrateHandler::parse(const QString &text)
{
    Invertebrate invertebrate;

    QRegularExpressionMatchIterator iter = curlyBraceElement.globalMatch(text);
    while(iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        QString matchingString = match.captured(1).replace("''", "\"");

        parseInfoboxToInvertebrate(matchingString, invertebrate);
    }

    return invertebrate;
}

InvertebrateHandler::~InvertebrateHandler()
{}
