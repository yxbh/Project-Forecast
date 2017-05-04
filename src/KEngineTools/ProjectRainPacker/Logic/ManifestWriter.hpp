#ifndef MANIFESTWRITER_HPP
#define MANIFESTWRITER_HPP

#include "Context.hpp"

#include "KEngine/Common/Json.hpp"

class ManifestWriter
{
public:
    ManifestWriter() = delete;

    static void write(const Context & context, const QString & outputPath);
    static ke::Json toJson(ke::ResourceSptr resource);
};

#endif // MANIFESTWRITER_HPP
