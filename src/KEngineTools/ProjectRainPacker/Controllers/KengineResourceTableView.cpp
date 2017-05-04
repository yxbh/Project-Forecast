#include "KengineResourceTableView.hpp"

#include "Entities/Resources.hpp"
#include "Logic/Constants.hpp"
#include "Logic/UrlResourceTypeDetector.hpp"
#include "Models/Models.hpp"

#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>

KengineResourceTableView::KengineResourceTableView(QWidget *parent)
    : QTableView(parent)
{
}

void KengineResourceTableView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "KengineResourceTableView::dragEnterEvent()";
    if (event->mimeData()->hasUrls())
    {
//        qDebug() << "accept";
        event->acceptProposedAction();
    }
}

void KengineResourceTableView::dropEvent(QDropEvent *event)
{
    qDebug() << "dropEvent()";

    auto model = this->model();
    if (!model)
    {
        return;
    }

    auto kresourceModel = dynamic_cast<KengineResourceModel*>(model);
    if (!kresourceModel)
    {
        qDebug() << "Unexpected model connected to " << this->metaObject()->className();
        return;
    }

    auto urls = event->mimeData()->urls();
    for (const auto url : urls)
    {
        auto resourceName = url.path().split("/").last().split("\\").last().split(".").first().toStdString();
        auto resourceType = UrlResourceTypeDetector::toResourceType(url).toStdString();

        if (resourceType == ResourceTypes::Image)
        {
            auto newResource = ke::makeResource<ImageResource>();
            newResource->setName(resourceName);
            newResource->setSourcePath(url.path().toStdString());
            kresourceModel->addResource(newResource);
        }
        else
        {
            auto newResource = std::make_shared<KengineResource>();
            newResource->setName(resourceName);
            newResource->setSourcePath(url.path().toStdString());
            newResource->setType(resourceType);
            kresourceModel->addResource(newResource);
        }
    }

    if (!urls.isEmpty())
    {
        qDebug() << urls.length() << " resource(s) added to model";
    }
}
