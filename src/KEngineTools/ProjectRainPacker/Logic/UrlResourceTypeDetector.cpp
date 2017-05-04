#include "UrlResourceTypeDetector.hpp"

#include "Logic/Constants.hpp"

#include <QFileInfo>

QString UrlResourceTypeDetector::toResourceType(const QUrl & url)
{
    QFileInfo fileInfo(url.path());
    const auto && fileExtension = fileInfo.suffix().toLower();
    if (fileExtension == "km" ||
        fileExtension == "kem" ||
        fileExtension == "kmanifest")
    {
        return ResourceTypes::Manifest;
    }
    else if (fileExtension == "png")
    {
        return ResourceTypes::Image;
    }
    else if (fileExtension == "mp3")
    {
        return ResourceTypes::Audio;
    }

    return ResourceTypes::Unknown;
}
