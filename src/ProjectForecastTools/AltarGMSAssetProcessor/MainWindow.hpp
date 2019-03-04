#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void updateProcessImageAssetButton();

private slots:
    void on_pushButton_browseBackgroundFolder_clicked();
    void on_pushButton_browseSpriteFolder_clicked();
    void on_pushButton_browseTexpageFolder_clicked();
    void on_pushButton_browseTextureFolder_clicked();
    void on_pushButton_imageProcessingOutputFolder_clicked();
    void on_lineEdit_backgroundFolderPath_textChanged(const QString &);
    void on_lineEdit_spriteFolderPath_textChanged(const QString &);
    void on_lineEdit_texpageFolderPath_textChanged(const QString &);
    void on_lineEdit_textureFolderPath_textChanged(const QString &);

    void on_pushButton_processImageAssets_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
