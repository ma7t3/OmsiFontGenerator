#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OmsiChar.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QFontDialog>
#include <QPicture>
#include <QPixmap>
#include <QPainter>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateCharsetFont(const QFont &newFont);
    void updatePreview();
    QPicture drawPicture(const bool &debugMode);

    QPixmap renderPixmap();
    QString getOmsiFontFileContent() const;

    void on_pbExportBMP_clicked();
    void on_actionExport_Font_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionViewOnGitHub_triggered();

    void on_actionAbout_triggered();

    void on_actionExportBmp_triggered();

    void on_actionAboutQt_triggered();

private:
    Ui::MainWindow *ui;

    QFont _defaultFont;

    QFontDialog *_fontSelector;
    QList<OmsiChar> _chars;

    QString _currentSavePath;

    int _currentFontHeight = 0;
};
#endif // MAINWINDOW_H
