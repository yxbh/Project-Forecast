#ifndef URLRESOURCETYPEDETECTOR_HPP
#define URLRESOURCETYPEDETECTOR_HPP

#include <QString>
#include <QUrl>

class UrlResourceTypeDetector
{
public:
    UrlResourceTypeDetector() = delete;

    static QString toResourceType(const QUrl & url);
};

#endif // URLRESOURCETYPEDETECTOR_HPP
