#include "FontPixmapPreview.h"

FontPixmapPreview::FontPixmapPreview(QWidget *parent) : QWidget(parent) {}

void FontPixmapPreview::paintEvent(QPaintEvent *e) {
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.drawPicture(0, 0, _picture);
    p.end();
    setMinimumSize(_picture.boundingRect().size());
}

void FontPixmapPreview::setPicture(const QPicture &pic) {
    _picture = pic;
    update();
}
