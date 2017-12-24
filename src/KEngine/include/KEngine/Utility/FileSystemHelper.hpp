#pragma once

#include <exception>
#include <filesystem>
#include <vector>

namespace ke
{
    namespace fs = std::experimental::filesystem::v1;

    class FileSystemHelper
    {
    public:
        using PathContainer = std::vector<fs::path>;

        FileSystemHelper() = delete;

        /// <summary>
        /// Count the number of files in the given directory path.
        /// </summary>
        /// <param name="directoryPath"></param>
        /// <param name="recursive"></param>
        /// <returns></returns>
        inline static bool countFiles(const fs::path & directoryPath, bool recursive = false)
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

        /// <summary>
        /// Get all file paths under the given directory path.
        /// </summary>
        /// <param name="directoryPath"></param>
        /// <param name="recursive"></param>
        /// <returns></returns>
        inline static PathContainer getFilePaths(const fs::path & directoryPath, bool recursive = false)
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

        /// <summary>
        /// Get all paths under the given directory path.
        /// The paths could be files, folders and etc.
        /// </summary>
        /// <param name="directoryPath"></param>
        /// <param name="recursive"></param>
        /// <returns></returns>
        inline static PathContainer getChildPaths(const fs::path & directoryPath, bool recursive = false)
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
    };

}