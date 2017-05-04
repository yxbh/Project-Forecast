#ifndef MANIFESTGENERATIONDIALOG_HPP
#define MANIFESTGENERATIONDIALOG_HPP

#include "Logic/Context.hpp"

#include <QDialog>

namespace Ui {
class ManifestGenerationDialog;
}

class ManifestGenerationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManifestGenerationDialog(Context * context, QWidget *parent = nullptr);
    ~ManifestGenerationDialog();

private slots:
    void on_pushButton_generateManifest_clicked();
    void on_pushButton_browseManifestOutputPath_clicked();
    void on_pushButton_saveOptionsStates_clicked();

private:
    void saveOptionsStates();

    Ui::ManifestGenerationDialog *ui;

    Context * context;
};

#endif // MANIFESTGENERATIONDIALOG_HPP
