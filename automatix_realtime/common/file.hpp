#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

namespace amx
{
    class file
    {
    public:
        static std::string read_all(const std::string& path, std::ios::openmode Mode = std::ios::binary | std::ios::in)
        {
            std::fstream is(path, Mode);
            if (is.is_open())
            {
                // get length of file:
                is.seekg(0, is.end);
                size_t length = static_cast<size_t>(is.tellg());
                is.seekg(0, is.beg);

                std::string tmp;
                tmp.resize(length);
                is.read(&tmp.front(), length);
                is.close();
                return tmp;
            }
            return std::string();
        }

        static bool write(const std::string& path, const std::string& content, std::ios::openmode Mode = std::ios::out)
        {
            std::fstream os(path, Mode);
            if (os.is_open())
            {
                os.write(content.data(), content.size());
                os.close();
                return true;
            }
            return false;
        }

        static size_t get_file_size(const std::string& path)
        {
            std::ifstream is(path, std::ifstream::binary);
            if (is) {
                // get length of file:
                is.seekg(0, is.end);
                auto length = static_cast<size_t>(is.tellg());
                is.close();
                return length;
            }
            return 0;
        }

        static std::filesystem::path get_exe_directory()
        {
#ifdef _WIN32
            // Windows specific
            wchar_t sz_path[MAX_PATH];
            GetModuleFileNameW(NULL, sz_path, MAX_PATH);
#else
            // Linux specific
            char sz_path[PATH_MAX];
            ssize_t count = readlink("/proc/self/exe", sz_path, PATH_MAX);
            if (count < 0 || count >= PATH_MAX)
                return {}; // some error
            sz_path[count] = '\0';
#endif
            return std::filesystem::path{ sz_path }.parent_path() / ""; 
        }
    };
}

