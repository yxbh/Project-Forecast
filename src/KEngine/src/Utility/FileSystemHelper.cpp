#include "KEngine/Utility/FileSystemHelper.hpp"

namespace ke
{

    bool FileSystemHelper::countFiles(const fs::path & directoryPath, bool recursive)
    {
        if (!fs::is_directory(directoryPath))
        {
            throw std::invalid_argument("FileSystemHelper::countFiles(), path is not a directory.");
        }

        return
            recursive
            ?
            std::count_if(
                fs::recursive_directory_iterator{ directoryPath }, fs::recursive_directory_iterator{},
                static_cast<bool(*)(const fs::path &)>(fs::is_regular_file))
            :
            std::count_if(
                fs::directory_iterator{ directoryPath }, fs::directory_iterator{},
                static_cast<bool(*)(const fs::path &)>(fs::is_regular_file));
    }

    FileSystemHelper::PathContainer FileSystemHelper::getFilePaths(const fs::path & directoryPath, bool recursive)
    {
        if (!fs::is_directory(directoryPath))
        {
            throw std::invalid_argument("FileSystemHelper::getFilePaths(), path is not a directory.");
        }

        PathContainer paths;
        if (recursive)
        {
            fs::recursive_directory_iterator itr{ directoryPath }, end;
            std::for_each(itr, end, [&paths](const fs::directory_entry & cur) { if (fs::is_regular_file(cur.path())) paths.push_back(cur.path()); });
        }
        else
        {
            fs::directory_iterator itr{ directoryPath }, end;
            std::for_each(itr, end, [&paths](const fs::directory_entry & cur) { if (fs::is_regular_file(cur.path())) paths.push_back(cur.path()); });
        }
        return paths;
    }

    FileSystemHelper::PathContainer FileSystemHelper::getChildPaths(const fs::path & directoryPath, bool recursive)
    {
        if (!fs::is_directory(directoryPath))
        {
            throw std::invalid_argument("FileSystemHelper::getChildPaths(), path is not a directory.");
        }

        PathContainer paths;
        if (recursive)
        {
            fs::recursive_directory_iterator itr{ directoryPath }, end;
            std::for_each(itr, end, [&paths](const fs::directory_entry & cur) { paths.push_back(cur.path()); });
        }
        else
        {
            fs::directory_iterator itr{ directoryPath }, end;
            std::for_each(itr, end, [&paths](const fs::directory_entry & cur) { paths.push_back(cur.path()); });
        }
        return paths;
    }

}