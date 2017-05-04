#include "ManifestGenerationDialog.hpp"
#include "ui_ManifestGenerationDialog.h"

#include <QFileDialog>

ManifestGenerationDialog::ManifestGenerationDialog(Context * p_context, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManifestGenerationDialog),
    context(p_context)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
}

ManifestGenerationDialog::~ManifestGenerationDialog()
{
    delete ui;
}

void ManifestGenerationDialog::on_pushButton_generateManifest_clicked()
{
    this->saveOptionsStates();
}

void ManifestGenerationDialog::on_pushButton_browseManifestOutputPath_clicked()
{
    auto path = QFileDialog::getSaveFileName(this, "Manifest Output");
    if (path.isEmpty())
    {
        return;
    }

    this->ui->lineEdit_manifestOutputPath->setText(path);
}

void ManifestGenerationDialog::on_pushButton_saveOptionsStates_clicked()
{
    this->saveOptionsStates();
}

void ManifestGenerationDialog::saveOptionsStates()
{
    this->context->manifestOutputPath = this->ui->lineEdit_manifestOutputPath->text();
}
