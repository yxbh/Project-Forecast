#pragma once

#include <QStringList>

#include <functional>
#include <unordered_map>

namespace KengineJsonTypes
{
    static constexpr const char * const Resource = "resource";
}

namespace KengineResourceProperties
{

static constexpr const char * const Name = "Name";
static constexpr const char * const Type = "Type";
static constexpr const char * const SourcePath = "Source Path";

}

namespace ResourceTypes
{
    static constexpr const char * const Manifest  = "Manifest";
    static constexpr const char * const Image     = "Image";
    static constexpr const char * const Audio     = "Audio";
    static constexpr const char * const GmsRoom   = "GM:S Room";
    static constexpr const char * const Unknown   = "Unknown";
}

extern const QStringList ResourceTypeNames;

class QStringHasher
{
public:
    size_t operator() (QString const& key) const
      {
          return qHash(key);
      }
};

using QStringIntMap = std::unordered_map<QString, int, QStringHasher>;
extern const QStringIntMap ResourceTypeEnumValueMap;

namespace ManifestKeys
{
    static constexpr const char * const Type = "type";
    static constexpr const char * const RainPackerMetaData = "rainpacker_meta_data";
    static constexpr const char * const WorkspaceManifestOutputPath = "workspace_manifest_output_path";
    static constexpr const char * const AdditionalManifestPaths = "additional_manifest_paths";
    static constexpr const char * const Resources = "resources";

}
