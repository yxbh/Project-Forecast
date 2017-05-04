#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Models/Models.hpp"

struct Context
{
    KengineResourceModel * resourceModel;
    QString currentWorkspaceManifestPath;
    QString manifestOutputPath;
};

#endif // CONTEXT_HPP
