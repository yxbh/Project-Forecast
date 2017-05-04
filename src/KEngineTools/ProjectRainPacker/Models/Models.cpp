#include "Models.hpp"

#include "Entities/Resources.hpp"
#include "Logic/Constants.hpp"

#include <QDebug>
#include <QMimeData>

#include <algorithm>
#include <cassert>


KengineResourceModel::KengineResourceModel(QObject *parent)
    : ModelBaseClass(parent)
{
    this->setupHorizontalHeaderData();
}

KengineResourceModel::KengineResourceModel(int rows, int columns, QObject *parent)
 : ModelBaseClass(rows, columns, parent)
{
    this->setupHorizontalHeaderData();
}

void KengineResourceModel::setupHorizontalHeaderData()
{
    this->setHorizontalHeaderItem(Column::Name, new QStandardItem(KengineResourceProperties::Name));
    this->setHorizontalHeaderItem(Column::Type, new QStandardItem(KengineResourceProperties::Type));
    this->setHorizontalHeaderItem(Column::SourcePath, new QStandardItem(KengineResourceProperties::SourcePath));
}

void KengineResourceModel::addResource(ke::ResourceSptr resource)
{
    auto newRowIdx = this->rowCount();
    this->setItem(newRowIdx, Column::Name, new ResourceItem(resource->getName().c_str(), resource));
    this->setItem(newRowIdx, Column::Type, new QStandardItem(resource->getType().c_str()));
    this->setItem(newRowIdx, Column::SourcePath, new QStandardItem(resource->getSourcePath().c_str()));
}

void KengineResourceModel::setResource(int row, ke::ResourceSptr resource)
{
    qDebug() << "KengineResourceModel::setResource()";
    this->setItem(row, Column::Name, new ResourceItem(resource->getName().c_str(), resource));
    this->setItem(row, Column::Type, new QStandardItem(resource->getType().c_str()));
    this->setItem(row, Column::SourcePath, new QStandardItem(resource->getSourcePath().c_str()));
}

ke::ResourceSptr KengineResourceModel::getResource(int rowIndex)
{
    return static_cast<ResourceItem*>(this->item(rowIndex, Column::Name))->getResource();
}

Qt::DropActions KengineResourceModel::supportedDropActions() const
{
//    qDebug () << "supportedDropActions()";
    auto actions = ModelBaseClass::supportedDropActions();
    actions |= /*Qt::DropAction::CopyAction | Qt::DropAction::MoveAction |*/ Qt::DropAction::LinkAction;
    return actions;
}

QStringList KengineResourceModel::mimeTypes() const
{
//    qDebug () << "mimeTypes()";
    auto list = ModelBaseClass::mimeTypes();
    list << "text/plain"
         << "application/octet-stream"
         << "text/uri-list"
         << "application/x-qabstractitemmodeldatalist";
    return list;
}

bool KengineResourceModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
//    qDebug () << "dropMimeData()";

    if (action == Qt::DropAction::IgnoreAction)
    {
        return true;
    }

    return ModelBaseClass::dropMimeData(data, action, row, column, parent);
}
