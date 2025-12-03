#ifndef DLGPREFERENCES_H
#define DLGPREFERENCES_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class DlgPreferences;
}

class DlgPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPreferences(QWidget *parent = nullptr);
    ~DlgPreferences();

public slots:
    void accept() override;

protected:
    void loadPreferences();
    void savePreferences();

private slots:
    void on_pbOmsiDirBrowse_clicked();

private:
    Ui::DlgPreferences *ui;

    QSettings _set;
};

#endif // DLGPREFERENCES_H
