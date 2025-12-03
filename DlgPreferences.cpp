#include "DlgPreferences.h"
#include "ui_DlgPreferences.h"

#include <QFileDialog>

#include "Metadata.h"

DlgPreferences::DlgPreferences(QWidget *parent) : QDialog(parent), ui(new Ui::DlgPreferences), _set(Metadata::authorName(), Metadata::applicationName()) {
    ui->setupUi(this);
    loadPreferences();
}

DlgPreferences::~DlgPreferences() {
    delete ui;
}

void DlgPreferences::accept() {
    savePreferences();
    QDialog::accept();
}

void DlgPreferences::loadPreferences() {
    ui->cbLanguage->setCurrentIndex(_set.value("language", 0).toInt());
    ui->leOmsiDir->setText(_set.value("omsiDir", "C:/Program Files (x86)/Steam/steamapps/common/OMSI 2").toString());
}

void DlgPreferences::savePreferences() {
    _set.setValue("language", ui->cbLanguage->currentIndex());
    _set.setValue("omsiDir", ui->leOmsiDir->text());
}

void DlgPreferences::on_pbOmsiDirBrowse_clicked() {
    const QString path = QFileDialog::getExistingDirectory(this, "", ui->leOmsiDir->text());
    if(!path.isEmpty())
        ui->leOmsiDir->setText(path);
}
