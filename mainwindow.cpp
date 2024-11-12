#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _fontSelector(new QFontDialog(this)) {
    ui->setupUi(this);

    showMaximized();

    _fontSelector->setOption(QFontDialog::NoButtons);
    _fontSelector->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    ui->gbFont->layout()->addWidget(_fontSelector);

    _defaultFont.setPointSize(48);
    _fontSelector->setCurrentFont(_defaultFont);

    // connect-commands
    connect(_fontSelector, &QFontDialog::currentFontChanged, this, &MainWindow::updatePreview);
    connect(ui->pteCharset, &QPlainTextEdit::textChanged, this, &MainWindow::updatePreview);

    connect(ui->sbCharSpacing, &QSpinBox::valueChanged, this, &MainWindow::updatePreview);
    connect(ui->sbLineSpacing, &QSpinBox::valueChanged, this, &MainWindow::updatePreview);

    connect(ui->sbExtraAscent, &QSpinBox::valueChanged, this, &MainWindow::updatePreview);
    connect(ui->sbExtraDescent, &QSpinBox::valueChanged, this, &MainWindow::updatePreview);
    connect(ui->sbExtraSidemargin, &QSpinBox::valueChanged, this, &MainWindow::updatePreview);

    connect(ui->cbDrawBoundingboxes, &QCheckBox::stateChanged, this, &MainWindow::updatePreview);
    connect(ui->cbDrawGap, &QCheckBox::stateChanged, this, &MainWindow::updatePreview);
    connect(ui->cbDisableAntialais, &QCheckBox::stateChanged, this, &MainWindow::updatePreview);

    connect(ui->cbEqualNumberWidth, &QCheckBox::stateChanged, this, &MainWindow::updatePreview);

    connect(ui->twChars, &QTreeWidget::currentItemChanged, this, &MainWindow::updatePreview);

    connect(ui->pbExportFont, &QPushButton::clicked, this, &MainWindow::on_actionExport_Font_triggered);

    updatePreview();

    ui->twChars->setColumnWidth(0, 20);
    ui->twChars->setColumnWidth(1, 20);
    ui->twChars->setColumnWidth(2, 20);
    ui->twChars->setColumnWidth(3, 20);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updatePreview() {
    ui->fontPixmapPreview->setPicture(drawPicture(true));
}

QPicture MainWindow::drawPicture(const bool &debugMode) {
    int currentCharIndex = ui->twChars->currentIndex().row();

    ui->twChars->clear();
    _chars.clear();

    QPicture pic;
    QPainter p(&pic);

    int lineSpacing = ui->sbLineSpacing->value();
    int charSpacing = ui->sbCharSpacing->value();
    int addAscent = ui->sbExtraAscent->value();
    int addDescent = ui->sbExtraDescent->value();
    int extraSideMargin = ui->sbExtraSidemargin->value();

    QString charset = ui->pteCharset->toPlainText();
    QStringList charsetLines = charset.split("\n");
    QFont font = _fontSelector->currentFont();
    QFontMetrics fm(font);

    if(ui->cbDisableAntialais->isChecked())
        font.setStyleHint(QFont::AnyStyle, QFont::NoAntialias);

    _currentFontHeight = fm.height() + addAscent + addDescent;

    int numberWidth = 0;
    bool equalNumberWidth = ui->cbEqualNumberWidth->isChecked();
    const QStringList numbers = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    if(equalNumberWidth) {
        for(QString ch : numbers)
            numberWidth = qMax(numberWidth, fm.boundingRect(ch).width());
    }

    // Calculate space width
    int spaceWidth = fm.horizontalAdvance("X X") - fm.horizontalAdvance("XX");

    int maxWidth = 0;
    for(QString line : charsetLines) {
        int currentLineWidth = 0;
        for(QString ch : line.split("")) {
            if(ch == " ")
                currentLineWidth += spaceWidth + charSpacing + (extraSideMargin * 2);
            else if(equalNumberWidth && numbers.contains(ch))
                currentLineWidth += numberWidth + charSpacing + (extraSideMargin * 2);
            else
                currentLineWidth += fm.boundingRect(ch).width() + charSpacing + (extraSideMargin * 2);
        }

        if(maxWidth < currentLineWidth)
            maxWidth = currentLineWidth;
    }

    int picHeight = (fm.height() + addAscent + addDescent + lineSpacing) * charsetLines.count();

    if(ui->cbDrawGap->isChecked())
        p.setBrush(Qt::red);
    else
        p.setBrush(Qt::black);

    p.drawRect(0, 0, maxWidth, picHeight);

    p.setBrush(Qt::black);
    p.setPen(Qt::white);
    p.setFont(font);

    int currentX = 0, currentY = fm.ascent();

    int i = 0;

    for(QString line : charsetLines) {
        for(QString ch : line.split("")) {
            if(ch.isEmpty())
                continue;

            QRect r = fm.boundingRect(ch);

            if(ch == " ")
                r.setWidth(spaceWidth);

            int oldNumberWidthDiff = 0;
            if(equalNumberWidth && numbers.contains(ch)) {
                int oldNumberWidth = r.width();
                r.setWidth(numberWidth);
                oldNumberWidthDiff = r.width() - oldNumberWidth;
            }

            r.setWidth(r.width() + (2 * extraSideMargin));

            if(ui->cbDrawBoundingboxes->isChecked() && debugMode) {
                p.setPen(Qt::red);
            } else {
                p.setPen(Qt::NoPen);
            }

            if(currentCharIndex == i && debugMode)
                p.setBrush(Qt::blue);

            QRect fixedR = r.translated(currentX - r.x(), currentY);
            fixedR.setHeight(fixedR.height() + addAscent + addDescent);

            p.drawRect(fixedR);

            if(currentCharIndex == i)
                p.setBrush(Qt::black);

            p.setPen(Qt::white);
            p.drawText(currentX - r.x() + (oldNumberWidthDiff / 2) + extraSideMargin, currentY + addAscent, ch);

            // Add to list
            QTreeWidgetItem *itm = new QTreeWidgetItem(ui->twChars);
            itm->setText(0, ch);
            itm->setText(1, QString::number(currentY - fm.ascent()));
            itm->setText(2, QString::number(currentX));
            itm->setText(3, QString::number(currentX + r.width()));
            ui->twChars->addTopLevelItem(itm);

            OmsiChar c(ch[0], currentX, currentX + r.width(), currentY);
            _chars << OmsiChar(ch[0], currentX, currentX + r.width(), currentY - fm.ascent());

            currentX += (r.width() + charSpacing);

            i++;
        }
        currentY += fm.height() + lineSpacing + addAscent + addDescent;
        currentX = 0;
    }

    return pic;
}

QPixmap MainWindow::renderPixmap() {
    QPicture pic = drawPicture(false);
    QPixmap pixmap(pic.width(), pic.height());
    QPainter p(&pixmap);
    p.drawPicture(0, 0, pic);
    return pixmap;
}

QString MainWindow::getOmsiFontFileContent() const {
    QString str;

    // Font header
    str += "[newfont]\r\n" + ui->leFontName->text() + "\r\n" + ui->leFileName->text() + ".bmp\r\n" + ui->leFileName->text() + ".bmp\r\n" + QString::number(_currentFontHeight) + "\r\n" + QString::number(ui->sbCharSpacing->value()) + "\r\n\r\n";

    // Chars
    for(OmsiChar ch: _chars)
        str += "[char]\r\n" + QString(ch.character) + "\r\n" + QString::number(ch.leftPixel) + "\r\n" + QString::number(ch.rightPixel) + "\r\n" + QString::number(ch.topPixel) + "\r\n\r\n";

    return str;
}

void MainWindow::on_pbExportBMP_clicked() {
    QString filename = QFileDialog::getSaveFileName(this, "", _currentSavePath, "Bitmap (*.bmp)");
    if(filename.isEmpty())
        return;

    renderPixmap().save(filename);
}


void MainWindow::on_actionExport_Font_triggered() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Select your OMSI fonts directory"), "", QFileDialog::ShowDirsOnly);
    if(path.isEmpty())
        return;

    QString fileName = path + "/" + ui->leFileName->text();

    if(QFile::exists(fileName + ".bmp") || QFile::exists(fileName + ".oft")) {
        QMessageBox::StandardButton msg = QMessageBox::warning(this, tr("Font already exists"), tr("This Font already exists. Do you want to replace it?"), QMessageBox::Yes|QMessageBox::No);
        if(msg != QMessageBox::Yes)
            return;
    }

    bool okPixmap = renderPixmap().save(fileName + ".bmp");
    if(!okPixmap) {
        QMessageBox::critical(this, tr("Couldn't save bitmap"), tr("Saving the font bitmap file failed."));
        return;
    }

    QFile oftFile(fileName + ".oft");
    if(!oftFile.open(QFile::WriteOnly)) {
        QMessageBox::critical(this, tr("Couldn't save oft file"), tr("Saving the font oft file failed."));
        return;
    }

    QTextStream s(&oftFile);
    s.setEncoding(QStringConverter::Latin1);
    s << getOmsiFontFileContent();
    oftFile.close();
}

void MainWindow::on_actionSave_triggered() {
    QString path = QFileDialog::getSaveFileName(this, "", _currentSavePath, "JSON File (*.json)");
    if(path.isEmpty())
        return;

    QFile f(path);
    if(!f.open(QFile::WriteOnly)) {
        QMessageBox::critical(this, tr("Couldn't save file"), tr("Failed saving the JSON file."));
        return;
    }

    QString charset = ui->pteCharset->toPlainText();
    int charSpace = ui->sbCharSpacing->value();
    int lineSpace = ui->sbLineSpacing->value();
    int addAsc = ui->sbExtraAscent->value();
    int addDesc = ui->sbExtraDescent->value();
    int extraSideMargin = ui->sbExtraSidemargin->value();
    QString font = _fontSelector->currentFont().toString();
    QString fontName = ui->leFontName->text();
    QString fontFileName = ui->leFileName->text();
    int fontCharGap = ui->sbGapBetweenCharacters->value();
    bool equalNumberWidth = ui->cbEqualNumberWidth->isChecked();

    QJsonDocument doc;
    QJsonObject fontObj = {
        {"font", font},
        {"fontName", fontName},
        {"fontFileName", fontFileName},
        {"fontCharGap", fontCharGap}
    };
    QJsonObject obj = {
        {"charset", charset},
        {"charSpace", charSpace},
        {"lineSpace", lineSpace},
        {"addAsc", addAsc},
        {"addDesc", addDesc},
        {"extraSideMargin", extraSideMargin},
        {"equalNumberWidth", equalNumberWidth},
        {"font", fontObj}
    };

    doc.setObject(obj);

    f.write(doc.toJson(QJsonDocument::Indented));
    f.close();

    _currentSavePath = path;
}

void MainWindow::on_actionOpen_triggered() {
    QString path = QFileDialog::getOpenFileName(this, "", _currentSavePath, "JSON File (*.json)");
    if(path.isEmpty())
        return;

    on_actionNew_triggered();

    QFile f(path);
    if(!f.open(QFile::ReadOnly)) {
        QMessageBox::critical(this, tr("Couldn't read file"), tr("Failed reading the JSON file."));
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &error);

    if(error.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, tr("Couldn't parse JSON"), tr("Failed parsing JSON:<br>%1 at byte %2").arg(error.errorString()).arg(QString::number(error.offset)));
        return;
    }

    QJsonObject obj = doc.object();

    ui->pteCharset->setPlainText(obj.value("charset").toString(""));
    ui->sbCharSpacing->setValue(obj.value("charSpace").toInt(0));
    ui->sbLineSpacing->setValue(obj.value("lineSpace").toInt(0));
    ui->sbExtraAscent->setValue(obj.value("addAsc").toInt(0));
    ui->sbExtraDescent->setValue(obj.value("addDesc").toInt(0));
    ui->sbExtraSidemargin->setValue(obj.value("extraSideMargin").toInt(0));
    ui->cbEqualNumberWidth->setChecked(obj.value("equalNumberWidth").toBool(false));

    QJsonObject fontObj = obj.value("font").toObject();

    QFont font;
    font.fromString(fontObj.value("font").toString(""));
    _fontSelector->setCurrentFont(font);

    ui->leFontName->setText(fontObj.value("fontName").toString(""));
    ui->leFileName->setText(fontObj.value("fontFileName").toString(""));
    ui->sbGapBetweenCharacters->setValue(fontObj.value("fontCharGap").toInt(0));
}

void MainWindow::on_actionNew_triggered() {
    _currentSavePath.clear();

    ui->pteCharset->clear();
    ui->sbCharSpacing->setValue(ui->sbCharSpacing->minimum());
    ui->sbLineSpacing->setValue(ui->sbLineSpacing->minimum());
    ui->sbExtraAscent->setValue(0);
    ui->sbExtraDescent->setValue(0);
    ui->sbExtraSidemargin->setValue(0);
    ui->cbEqualNumberWidth->setChecked(false);
    _fontSelector->setCurrentFont(_defaultFont);
    ui->leFontName->clear();
    ui->leFileName->clear();
    ui->sbGapBetweenCharacters->setValue(0);
}
