#ifndef FONTPIXMAPPREVIEW_H
#define FONTPIXMAPPREVIEW_H

#include <QWidget>
#include <QPicture>
#include <QPainter>

class FontPixmapPreview : public QWidget
{
    Q_OBJECT
public:
    explicit FontPixmapPreview(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;

    void setPicture(const QPicture &);

signals:

private:
    QPicture _picture;
};

#endif // FONTPIXMAPPREVIEW_H
