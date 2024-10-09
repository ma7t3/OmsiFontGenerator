#ifndef OMSICHAR_H
#define OMSICHAR_H

#include <QChar>

class OmsiChar {
public:
    OmsiChar(const QChar &character, const int &leftPixel, const int &rightPixel, const int &topPixel) :
        character(character),
        leftPixel(leftPixel),
        rightPixel(rightPixel),
        topPixel(topPixel) {
    }

    QChar character;
    int leftPixel;
    int rightPixel;
    int topPixel;
};

#endif // OMSICHAR_H
