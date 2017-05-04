#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Models/Models.hpp"

#include <QStringList>

struct Context
{
    KengineResourceModel * resourceModel;
    QString currentWorkspaceManifestPath;
    QString manifestOutputPath;

    QStringList errorMessages;
};

#endif // CONTEXT_HPP
