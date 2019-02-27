#include "KEngine/Utility/FileSystemHelper.hpp"

#include <algorithm>
#include <cassert>

namespace ke
{

    static constexpr size_t VEC_RESERVE_MIN = 4096;

    bool FileSystemHelper::countFiles(const fs::path & directoryPath, bool recursive)
    {
        assert(fs::exists(directoryPath));
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
        assert(fs::exists(directoryPath));
        if (!fs::is_directory(directoryPath))
        {
            throw std::invalid_argument("FileSystemHelper::getFilePaths(), path is not a directory.");
        }

        PathContainer paths;
        paths.reserve(VEC_RESERVE_MIN);
        auto is_file = [](const fs::directory_entry & dt) { return fs::is_regular_file(dt); };
        if (recursive)
        {
            fs::recursive_directory_iterator itr{ directoryPath }, end;
            std::copy_if(itr, end, std::back_inserter(paths), is_file);
        }
        else
        {
            fs::directory_iterator itr{ directoryPath }, end;
            std::copy_if(itr, end, std::back_inserter(paths), is_file);
        }
        return paths;
    }

    FileSystemHelper::PathContainer FileSystemHelper::getChildPaths(const fs::path & directoryPath, bool recursive)
    {
        assert(fs::exists(directoryPath));
        if (!fs::is_directory(directoryPath))
        {
            throw std::invalid_argument("FileSystemHelper::getChildPaths(), path is not a directory.");
        }

        if (recursive)
        {
            fs::recursive_directory_iterator itr{ directoryPath }, end;
            return PathContainer(itr, end);
        }
        fs::directory_iterator itr{ directoryPath }, end;
        return PathContainer(itr, end);
    }

}