#ifndef STREAMHANDLER_H
#define STREAMHANDLER_H

#include <QString>
#include <QRegularExpression>
#include <QDebug>

#include "../models/stream.h"

// Example data follows

/*
{{Rivers/ProjectNav}}

{{Infobox stream |Stream = Baldwin Creek

|Stream code = OC_BldwnCrk_1026

|Basin = Otter Creek

|Latitude = 44.16231878

|Longitude = -73.01806138

|State or Province= Vermont

|Country = USA

|School = Mount Abraham Union High School

}}

The following are the most common invertebrates collected from this stream site.
&lt;!-------- ADD INSECT TEMPLATES BELOW HERE (after the greater than sign); CHIRONOMIDAE HAS BEEN ADDED TO ILLUSTRATE HOW IT'S DONE; OPEN A NEW TAB AND PASTE THIS LINK TO SHOW A FULL LIST OF THE AVAILABLE INVERTEBRATE TEMPLATES: http://wikieducator.org/Category:Aquatic_Insects   FEEL FREE TO PERSONALIZE THE PAGE AND LEAVE NOTES FOR YOUR COLLABORATORS, STUDENTS, AND YOUR TEACHERS ------&gt;
{{Baetis}}
{{Epeorus}}
{{Chironomidae}}
{{Simuliidae}}
{{Drunella}}
{{Chimarra}}
{{Leuctra}}
{{Dolophilodes}}
{{Antocha}}
{{Acroneuria}}

[[Category:Stream]]
*/

class StreamHandler
{
    // It would be far less fragile to write a parser for this instead of relying on Regex
    QRegularExpression curlyBraceElement;
    QString buffer;

    void parseInfoboxToStream(const QString &infoBox, Stream &stream);
public:
    StreamHandler();
    ~StreamHandler();
    Stream parse(const QString &text);
};

#endif // STREAMHANDLER_H
