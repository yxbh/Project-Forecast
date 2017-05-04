#include "ManifestWriter.hpp"

#include "Constants.hpp"
#include "Entities/Resources.hpp"

#include "KEngine/Common/Json.hpp"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

void ManifestWriter::write(const Context & context, const QString & outputPath)
{
    using namespace nlohmann;

    json jsonRoot;
    jsonRoot[ManifestKeys::Type] = "manifest";

    // RainPacker meta data
    json jsonRainPackerMeta;
    jsonRainPackerMeta[ManifestKeys::WorkspaceManifestOutputPath] = context.manifestOutputPath.toStdString();
    jsonRoot[ManifestKeys::RainPackerMetaData] = jsonRainPackerMeta;

    // additional manifest paths
    json jsonOtherManifests = json::array();
    jsonRoot[ManifestKeys::AdditionalManifestPaths] = jsonOtherManifests;

    // resources
    json jsonKeResources = json::array();
    for (int modelRow = 0; modelRow < context.resourceModel->rowCount(); ++modelRow)
    {
        ke::ResourceSptr resource = context.resourceModel->getResource(modelRow);
        if (nullptr == resource)
        {
            qDebug() << "Model is missing resource at row" << modelRow;
            continue;
        }
        jsonKeResources.push_back(toJson(resource));
    }
    jsonRoot[ManifestKeys::Resources] = jsonKeResources;

    QFile manifestFile(outputPath);
    manifestFile.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream stream(&manifestFile);
    stream << jsonRoot.dump(4).c_str();

}

ke::Json ManifestWriter::toJson(ke::ResourceSptr resource)
{
    if (auto resourceStrongTyped = std::dynamic_pointer_cast<ImageResource>(resource))
    {
        return resourceStrongTyped;
    }
    else if (auto resourceStrongTyped = std::dynamic_pointer_cast<KengineResource>(resource))
    {
        return resourceStrongTyped;
    }

    return {};
}
