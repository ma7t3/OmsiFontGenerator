#include "Metadata.h"

Metadata::Metadata() {}

QString Metadata::applicationName() {
    return "OmsiFontGenerator";
}

QString Metadata::versionName() {
    return "1.0.0";
}

QString Metadata::changeLog() {
    return ""; // TODO
}

QString Metadata::gitHubUrl() {
    return "https://github.com/ma7t3/OmsiFontGenerator";
}

QString Metadata::authorName() {
    return "ma7t3";
}

QString Metadata::authorUrl() {
    return "https://github.com/ma7t3";
}

QString Metadata::licenseName() {
    return "GPL-3.0";
}

QString Metadata::licenseUrl() {
    return "https://www.gnu.org/licenses/gpl-3.0.en.html";
}
