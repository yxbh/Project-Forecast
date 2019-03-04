#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "ImageAssetsProcessor.hpp"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateProcessImageAssetButton();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProcessImageAssetButton()
{
    ui->pushButton_processImageAssets->setEnabled(
                !ui->lineEdit_backgroundFolderPath->text().isEmpty() &&
                !ui->lineEdit_spriteFolderPath->text().isEmpty() &&
                !ui->lineEdit_texpageFolderPath->text().isEmpty() &&
                !ui->lineEdit_textureFolderPath->text().isEmpty() &&
                !ui->lineEdit_imageProcessingOutputFolderPath->text().isEmpty()
                );
}

void MainWindow::on_pushButton_browseBackgroundFolder_clicked()
{
    ui->lineEdit_backgroundFolderPath->setText(QFileDialog::getExistingDirectory(this, tr("Browse")));
}

void MainWindow::on_pushButton_browseSpriteFolder_clicked()
{
    ui->lineEdit_spriteFolderPath->setText(QFileDialog::getExistingDirectory(this, tr("Browse")));
}

void MainWindow::on_pushButton_browseTexpageFolder_clicked()
{
    ui->lineEdit_texpageFolderPath->setText(QFileDialog::getExistingDirectory(this, tr("Browse")));
}

void MainWindow::on_pushButton_browseTextureFolder_clicked()
{
    ui->lineEdit_textureFolderPath->setText(QFileDialog::getExistingDirectory(this, tr("Browse")));
}

void MainWindow::on_pushButton_imageProcessingOutputFolder_clicked()
{
    ui->lineEdit_imageProcessingOutputFolderPath->setText(QFileDialog::getExistingDirectory(this, tr("Browse")));
}

void MainWindow::on_lineEdit_backgroundFolderPath_textChanged(const QString &)
{
    updateProcessImageAssetButton();
}

void MainWindow::on_lineEdit_spriteFolderPath_textChanged(const QString &)
{
    updateProcessImageAssetButton();
}

void MainWindow::on_lineEdit_texpageFolderPath_textChanged(const QString &)
{
    updateProcessImageAssetButton();
}

void MainWindow::on_lineEdit_textureFolderPath_textChanged(const QString &)
{
    updateProcessImageAssetButton();
}

void MainWindow::on_pushButton_processImageAssets_clicked()
{
    int counter = 0;
    ke::AssetsProcessorSptr processor = ke::ImageAssetsProcessor::createProcessor(
                [this, &counter](const QString & msg){
                    if (++counter % 10 == 0)
                        qApp->processEvents();
                    ui->statusBar->showMessage(msg);
                },
                ui->lineEdit_backgroundFolderPath->text(),
                ui->lineEdit_spriteFolderPath->text(),
                ui->lineEdit_texpageFolderPath->text(),
                ui->lineEdit_textureFolderPath->text(),
                ui->lineEdit_imageProcessingOutputFolderPath->text(),
                qRgba(
                    ui->spinBox_imageProcessingBgFillColorChannelRed->value(),
                    ui->spinBox_imageProcessingBgFillColorChannelGreen->value(),
                    ui->spinBox_imageProcessingBgFillColorChannelBlue->value(),
                    ui->spinBox_imageProcessingBgFillColorChannelAlpha->value()
                ),
                ui->checkBox_imageProcessingOverrideAlpha0PixelWithFillColour->isChecked(),
                ui->checkBox_imageProcessingGenerateGif->isChecked(),
                ui->spinBox_imageProcessingGifFrameDelay->value());

    QString buttonTextOrg = ui->pushButton_processImageAssets->text();
    ui->pushButton_processImageAssets->setText("Processing");
    ui->pushButton_processImageAssets->setDisabled(true);

    if (processor->process())
    {
        ui->statusBar->showMessage(tr("Processing Successful"));
    }
    else
    {
        ui->statusBar->showMessage(tr("Processing unsuccessful"));
    }

    ui->pushButton_processImageAssets->setText(buttonTextOrg);
    ui->pushButton_processImageAssets->setEnabled(true);
}
