#include "ManifestEditor.hpp"

#include "Constants.hpp"
#include "Entities/Resources.hpp"

#include "KEngine/Common/Json.hpp"
#include "KEngine/Log/Log.hpp"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include <fstream>

namespace
{
    bool validateManifestJson(const ke::Json & manifestJson, const ke::String & manifestPath)
    {
        auto manifestTypeItr = manifestJson.find("type");
        if (manifestTypeItr == manifestJson.end())
        {
            ke::Log::instance()->error("Manifest JSON missing 'type' object.");
            return false;
        }
        if (manifestTypeItr->get<std::string>() != "manifest")
        {
            ke::Log::instance()->error("The JSON at '{}' is not an engine resource manifest.", manifestPath);
            return false;
        }

        return true;
    }

    bool validateResourceJson(const ke::Json & resourceJson)
    {
        auto typeItr = resourceJson.find("type");
        if (typeItr == resourceJson.end())
        {
            ke::Log::instance()->error("Engine resource JSON missing 'type'. Content: {}", resourceJson.dump(2));
            return false;
        }
        if (typeItr->get<std::string>() != "resource")
        {
            ke::Log::instance()->error("ENgine resource JSON has unexpected 'type' value. Content: {}", resourceJson.dump(2));
            return false;
        }

        auto resourceTypeItr = resourceJson.find("resource_type");
        if (resourceTypeItr == resourceJson.end())
        {
            ke::Log::instance()->error("Engine resource JSON missing 'resource_type'. Content: {}", resourceJson.dump(2));
            return false;
        }
        auto resourceType = resourceTypeItr->get<std::string>();

        auto resourceNameItr = resourceJson.find("resource_name");
        if (resourceNameItr == resourceJson.end())
        {
            ke::Log::instance()->error("Engine resource JSON missing 'resource_name'. Content: {}", resourceJson.dump(2));
            return false;
        }
        auto resourceName = resourceNameItr->get<std::string>();
        if (resourceName.length() == 0)
        {
            ke::Log::instance()->error("Engine resource JSON contains empty 'resource_name'. Content: {}", resourceJson.dump(2));
            return false;
        }

        return true;
    }
}

bool ManifestEditor::write(Context & context, const QString & outputPath)
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
            context.errorMessages << "Model is missing resource at row" + QString::number(modelRow);
            continue;
        }
        jsonKeResources.push_back(toJson(resource));
    }
    jsonRoot[ManifestKeys::Resources] = jsonKeResources;

    QFile manifestFile(outputPath);
    manifestFile.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream stream(&manifestFile);
    stream << jsonRoot.dump(3).c_str();

    return true;
}

bool ManifestEditor::read(Context & context, const QString & inputPath)
{
    // read the JSON content.
    std::ifstream ifs{ inputPath.toStdString() };
    std::string content( std::istreambuf_iterator<char> { ifs }, std::istreambuf_iterator<char>{} );
    auto manifestJson = ke::Json::parse(content.begin(), content.end());

    context.currentWorkspaceManifestPath = inputPath;
    context.resourceModel->clear();

    if (!::validateManifestJson(manifestJson, inputPath.toStdString()))
    {
        return false;
    }

    //  check if there are resources to load.
    auto resourcesItr = manifestJson.find("resources");
    if (resourcesItr == manifestJson.end())
    {
        context.errorMessages << QString("Resource manifest at '") + inputPath + "' does not have a 'resources' object.";
        return true;
    }

    // load all resources.
    for (const auto & resourceJson : *resourcesItr)
    {
        if (!::validateResourceJson(resourceJson))
        {
            context.errorMessages << QString("Invalid JSON: ") + resourceJson.dump(2).c_str();
            continue;
        }

        auto resourceName = resourceJson["resource_name"].get<std::string>();
//        if (this->allResources.find(resourceName) != this->allResources.end())
//        {
//            ke::Log::instance()->warn("Overwriting resource named '{}'.", resourceName);
//        }

        auto newResource = toResource(resourceJson);
        if (newResource)
        {
            context.resourceModel->addResource(newResource);
        }
        else
        {
            context.errorMessages << QString("Resource loading failed for ") + resourceJson.dump(2).c_str();
            return false;
        }
    }

    return true;
}

ke::Json ManifestEditor::toJson(ke::ResourceSptr resource)
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

ke::ResourceSptr ManifestEditor::toResource(const ke::Json & resourceJson)
{
    QString resourceType = resourceJson["resource_type"].get<std::string>().c_str();
    resourceType = resourceType.toLower();
    if (resourceType == QString(ResourceTypes::Image).toLower())
    {
        auto resource = ke::makeResource<ImageResource>();
        assert(resource);
        resource = resourceJson;
        return resource;
    }
    return nullptr;
}
