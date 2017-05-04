#pragma once

#include "KEngine/Interfaces/IResource.hpp"

#include <QString>

class ResourceManufacturer
{
public:
    ke::ResourceSptr create(QString resourceType);
};
