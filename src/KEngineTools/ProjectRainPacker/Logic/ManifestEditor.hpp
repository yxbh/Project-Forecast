#pragma once
#include "Context.hpp"

#include "KEngine/Common/Json.hpp"

class ManifestEditor
{
public:
    ManifestEditor() = delete;

    static bool write(Context & context, const QString & outputPath);
    static bool read(Context & context, const QString & inputPath);

    static ke::Json toJson(ke::ResourceSptr resource);
    static ke::ResourceSptr toResource(const ke::Json & resourceJson);
};
