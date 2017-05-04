#ifndef MODELS_HPP
#define MODELS_HPP

#include "Entities/Resources.hpp"

#include "KEngine/Interfaces/IResource.hpp"

#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QList>

class KengineResourceModel : public QStandardItemModel
{
    Q_OBJECT

public:
    using ModelBaseClass = QStandardItemModel;
    enum Column
    {
        Type = 0,
        Name = 1,
        SourcePath = 2,
        ColumnCount = 3
    };

    explicit KengineResourceModel(QObject *parent = Q_NULLPTR);
    KengineResourceModel(int rows, int columns, QObject *parent = Q_NULLPTR);
    virtual ~KengineResourceModel() {}

    void addResource(ke::ResourceSptr resource);
    void setResource(int row, ke::ResourceSptr resource);
    ke::ResourceSptr getResource(int rowIndex);

    virtual Qt::DropActions supportedDropActions() const override;
    virtual QStringList mimeTypes() const override;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

private:
    void setupHorizontalHeaderData();

protected:

    class ResourceItem : public QStandardItem
    {
    public:
        using QStandardItem::QStandardItem;

        ResourceItem(const QString & text, ke::ResourceSptr p_resource)
            : QStandardItem(text), resource(p_resource)
        {}

        inline ke::ResourceSptr getResource() { return this->resource; }

    private:
        ke::ResourceSptr resource;

    };

};


#endif // MODELS_HPP
