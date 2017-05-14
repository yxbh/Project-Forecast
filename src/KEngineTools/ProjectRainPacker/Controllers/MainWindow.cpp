#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include "Logic/Constants.hpp"
#include "Logic/ManifestEditor.hpp"
#include "Controllers/ManifestGenerationDialog.hpp"

#include "qtpropertymanager.h"
#include "qttreepropertybrowser.h"
#include "qteditorfactory.h"
#include "qtvariantproperty.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QTreeWidget>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setupUi();
    this->setupModels();

    this->context.resourceModel = &this->resourceModel;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
//    this->setAttribute(Qt::WA_DeleteOnClose);

    this->ui->setupUi(this);

    this->ui->splitter_main->setStretchFactor(0, 3);
    this->ui->splitter_main->setStretchFactor(1, 1);

    this->ui->tableView_allResources->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    this->ui->tableView_manifestResources->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    this->ui->tableView_imageResources->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    this->ui->tableView_audioResources->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    auto propertyBrowser = ui->widget_propertyBrowser;
    propertyBrowser->setResizeMode(QtTreePropertyBrowser::ResizeMode::Interactive);
    propertyBrowser->treeWidget()->resizeColumnToContents(0);
    this->updatePropertyBrowserContent(propertyBrowser, nullptr, -1);
}

void MainWindow::setupModels()
{
    this->manifestFilterModel.setSourceModel(&this->resourceModel);
    this->manifestFilterModel.setFilterKeyColumn(KengineResourceModel::Column::Type);
    this->manifestFilterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->manifestFilterModel.setFilterFixedString("manifest");
    this->imageFilterModel.setSourceModel(&this->resourceModel);
    this->imageFilterModel.setFilterKeyColumn(KengineResourceModel::Column::Type);
    this->imageFilterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->imageFilterModel.setFilterFixedString("image");
    this->audioFilterModel.setSourceModel(&this->resourceModel);
    this->audioFilterModel.setFilterKeyColumn(KengineResourceModel::Column::Type);
    this->audioFilterModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->audioFilterModel.setFilterFixedString("audio");

    this->ui->tableView_allResources->setModel(&this->resourceModel);
    this->ui->tableView_manifestResources->setModel(&this->manifestFilterModel);
    this->ui->tableView_imageResources->setModel(&this->imageFilterModel);
    this->ui->tableView_audioResources->setModel(&this->audioFilterModel);
}

void MainWindow::saveWorkspaceManifest(QString outputManifestPath)
{
    if (!ManifestEditor::write(this->context, outputManifestPath) ||
        this->context.errorMessages.count() > 0)
    {
        QMessageBox::critical(this, "Error",
            "There was a problem generating the manifest at '" + outputManifestPath + "'.\nError: " + this->context.errorMessages.join('\n'));
        this->context.errorMessages.clear();
    }
}

void MainWindow::loadWorkspaceManifest(QString inputManifestPath)
{
    if (!QFile(inputManifestPath).exists())
    {
        QMessageBox::critical(this, "Error", "The manifest path at '"+ inputManifestPath + "' does not exist.");
        return;
    }
    if (!ManifestEditor::read(this->context, inputManifestPath) ||
        this->context.errorMessages.count() > 0)
    {
        QMessageBox::critical(this, "Error",
            "There was a problem reading the manifest from '" + inputManifestPath + "'.\nError: " + this->context.errorMessages.join('\n'));
        this->context.errorMessages.clear();
    }
}

void MainWindow::updatePropertyBrowserContent(QtAbstractPropertyBrowser * propertyBrowser, ke::ResourceSptr resource, int row)
{
    auto properties = propertyBrowser->properties();
    for (auto property : properties)
    {
        propertyBrowser->removeProperty(property);
    }

    if (!resource)
    {
        propertyBrowser->setEnabled(false);
        return;
    }

    if (row < 0)
    {
        return;
    }
    qDebug() << "MainWindow::updatePropertyBrowserContent(), row:" << row;

    propertyBrowser->setEnabled(true);

    // reference: https://doc.qt.io/archives/qq/qq18-propertybrowser.html

    auto variantPropertyManager = new QtVariantPropertyManager(propertyBrowser);
    auto variantEditorFactory = new QtVariantEditorFactory(propertyBrowser);
    propertyBrowser->setFactoryForManager(variantPropertyManager, variantEditorFactory);

    auto generalGroupProperty = variantPropertyManager->addProperty(QtVariantPropertyManager::groupTypeId(), "General");
    auto nameProperty = variantPropertyManager->addProperty(QVariant::String, KengineResourceProperties::Name);
    auto pathProperty = variantPropertyManager->addProperty(QVariant::String, KengineResourceProperties::SourcePath);
    auto resourceTypeProperty = variantPropertyManager->addProperty(QtVariantPropertyManager::enumTypeId(), KengineResourceProperties::Type);
    resourceTypeProperty->setAttribute("enumNames", ResourceTypeNames);;

    propertyBrowser->addProperty(generalGroupProperty);
    generalGroupProperty->addSubProperty(nameProperty);
    generalGroupProperty->addSubProperty(pathProperty);
    generalGroupProperty->addSubProperty(resourceTypeProperty);

    // set property values.
    nameProperty->setValue(resource->getName().c_str());
    pathProperty->setValue(resource->getSourcePath().c_str());
    resourceTypeProperty->setValue(ResourceTypeEnumValueMap.at(resource->getType().c_str()));

    // ui stuff
    if (QtTreePropertyBrowser * treePropertyBrowser = dynamic_cast<QtTreePropertyBrowser*>(propertyBrowser))
    {
        treePropertyBrowser->treeWidget()->resizeColumnToContents(0);
    }

    variantPropertyManager->connect(variantPropertyManager, &QtVariantPropertyManager::valueChanged,
        [=](QtProperty *property, const QVariant &val){
            Q_UNUSED(val);
            auto variantProperty = static_cast<QtVariantProperty*>(property);
            qDebug() << "resource at row " << row << "changed.";
//            qDebug() << "variantProperty value:" << variantProperty->valueText();
//            qDebug() << "variant value:" << val;
            if (variantProperty->propertyName() == KengineResourceProperties::Name)
            {
                this->resourceModel.item(row, KengineResourceModel::Column::Name)->setText(property->valueText());
            }
            else if (variantProperty->propertyName() == KengineResourceProperties::Type)
            {
                this->resourceModel.item(row, KengineResourceModel::Column::Type)->setText(property->valueText());
            }
            else if (variantProperty->propertyName() == KengineResourceProperties::SourcePath)
            {
                this->resourceModel.item(row, KengineResourceModel::Column::SourcePath)->setText(property->valueText());
            }
        });
}

void MainWindow::showCurrentKengineResourceProperties(const QModelIndex & index)
{
//    qDebug() << "MainWindow::showCurrentKengineResourceProperties(), row:" << index.row();
    auto tempResource = this->resourceModel.getResource(index.row());
    if (this->currentResource == tempResource)
    {
        return;
    }
    this->currentResource = tempResource;
    auto propertyBrowser = this->ui->widget_propertyBrowser;
    this->updatePropertyBrowserContent(propertyBrowser, tempResource, index.row());
}

void MainWindow::on_tableView_allResources_activated(const QModelIndex &index)
{
    this->showCurrentKengineResourceProperties(index);
}

void MainWindow::on_tableView_allResources_clicked(const QModelIndex &index)
{
    this->showCurrentKengineResourceProperties(index);
}

void MainWindow::on_action_GenerateManifest_triggered()
{
    (new ManifestGenerationDialog(&this->context, this))->show();
}

void MainWindow::on_action_Save_triggered()
{
    if (this->context.currentWorkspaceManifestPath.isEmpty())
    {
        auto workspaceManifestSavePath = QFileDialog::getSaveFileName(
                    this, "Save Workspace Manifest", "", tr("Manifest (*.kem)"));
        if (workspaceManifestSavePath.isEmpty())
        {
            return;
        }

        this->context.currentWorkspaceManifestPath = workspaceManifestSavePath;
    }
    this->saveWorkspaceManifest(this->context.currentWorkspaceManifestPath);
}

void MainWindow::on_action_SaveAs_triggered()
{
    auto workspaceManifestSavePath = QFileDialog::getSaveFileName(
                this, "Save Workspace Manifest", this->context.currentWorkspaceManifestPath,
                tr("Manifest (*.kem)"));
    if (workspaceManifestSavePath.isEmpty())
    {
        return;
    }
    this->context.currentWorkspaceManifestPath = workspaceManifestSavePath;
    this->saveWorkspaceManifest(workspaceManifestSavePath);
}

void MainWindow::on_action_Open_triggered()
{
    auto workspaceManifestPath = QFileDialog::getOpenFileName(
                this, "Open Workspace Manifest", "", tr("Manifest (*.kem)"));
    if (workspaceManifestPath.isEmpty())
    {
        return;
    }
    this->loadWorkspaceManifest(workspaceManifestPath);
}

void MainWindow::on_action_Quit_triggered()
{
    this->close();
}
