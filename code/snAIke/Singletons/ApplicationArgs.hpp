#pragma once

#include <string_view>
#include <string>
#include <map>
#include <filesystem>
#include <snAIke/Singletons/Singleton.hpp>

#include <sstream>
#include <optional>

class ApplicationArgs
{
private:
    ApplicationArgs();
    ~ApplicationArgs() = default;
public:
    const char* Get(const std::string& arg);

    template <class T>
    std::optional<T> GetAs(const std::string& arg);

    std::filesystem::path GetApplicationLocation();

private:
    std::map<std::string, std::string> args;
    std::filesystem::path appPath;
private:
    friend class Singleton<ApplicationArgs>;
};

template<class T>
std::optional<T> ApplicationArgs::GetAs(const std::string& arg)
{
    if (auto option = Get(arg))
    {
        T temp;
        if (std::stringstream(option) >> temp)
        {
            return temp;
        }
    }
    return std::nullopt;
}
