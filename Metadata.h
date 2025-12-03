#ifndef METADATA_H
#define METADATA_H

#include <QString>

class Metadata {
public:
    Metadata();

    static QString applicationName();
    static QString versionName();
    static QString changeLog();
    static QString gitHubUrl();
    static QString authorName();
    static QString authorUrl();
    static QString licenseName();
    static QString licenseUrl();
};

#endif // METADATA_H
