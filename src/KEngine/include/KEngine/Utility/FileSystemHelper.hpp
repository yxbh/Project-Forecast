#pragma once

#include <vector>
#ifdef _MSC_VER
#include <filesystem>
#elif defined(__clang__)
#include <experimental/filesystem>
#endif

namespace ke
{

#ifdef _MSC_VER
    namespace fs = std::filesystem;
#elif defined(__clang__)
    namespace fs = std::filesystem;
#endif

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
        static bool countFiles(const fs::path & directoryPath, bool recursive = false);

        /// <summary>
        /// Get all file paths under the given directory path.
        /// </summary>
        /// <param name="directoryPath"></param>
        /// <param name="recursive"></param>
        /// <returns></returns>
        static PathContainer getFilePaths(const fs::path & directoryPath, bool recursive = false);

        /// <summary>
        /// Get all paths under the given directory path.
        /// The paths could be files, folders and etc.
        /// </summary>
        /// <param name="directoryPath"></param>
        /// <param name="recursive"></param>
        /// <returns></returns>
        static PathContainer getChildPaths(const fs::path & directoryPath, bool recursive = false);
    };

}