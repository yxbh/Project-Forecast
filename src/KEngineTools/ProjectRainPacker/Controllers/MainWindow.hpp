#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "Entities/Resources.hpp"
#include "Logic/Context.hpp"
#include "Models/Models.hpp"

#include <QMainWindow>
#include <QSortFilterProxyModel>

namespace Ui {
class MainWindow;
}

class QtAbstractPropertyBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void setupUi();
    void setupModels();

    void saveWorkspaceManifest(QString outputManifestPath);
    void loadWorkspaceManifest(QString inputManifestPath);

private slots:
    void on_tableView_allResources_activated(const QModelIndex &index);
    void on_tableView_allResources_clicked(const QModelIndex &index);
    void on_action_GenerateManifest_triggered();
    void on_action_Save_triggered();
    void on_action_SaveAs_triggered();
    void on_action_Open_triggered();
    void on_action_Quit_triggered();

private:
    void updatePropertyBrowserContent(QtAbstractPropertyBrowser * propertyBrowser, ke::ResourceSptr resourceItem, int row);
    void showCurrentKengineResourceProperties(const QModelIndex & index);

    Ui::MainWindow *ui;

    KengineResourceModel resourceModel;
    QSortFilterProxyModel manifestFilterModel;
    QSortFilterProxyModel imageFilterModel;
    QSortFilterProxyModel audioFilterModel;

    ke::ResourceSptr currentResource = nullptr;

    Context context;
};

#endif // MAINWINDOW_HPP
