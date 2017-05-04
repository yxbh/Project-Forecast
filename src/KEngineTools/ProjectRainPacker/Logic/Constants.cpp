#include "Constants.hpp"

const QStringList ResourceTypeNames
{
    ResourceTypes::Manifest,
    ResourceTypes::Image,
    ResourceTypes::Audio,
    ResourceTypes::GmsRoom,
    ResourceTypes::Unknown
};

const QStringIntMap ResourceTypeEnumValueMap
{
    { ResourceTypes::Manifest,  0 },
    { ResourceTypes::Image,     1 },
    { ResourceTypes::Audio,     2 },
    { ResourceTypes::GmsRoom,   3 },
    { ResourceTypes::Unknown,   4 }
};
