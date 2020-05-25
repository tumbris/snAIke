#include <snAIke/Singletons/ApplicationArgs.hpp>

extern char** g_argv;

const char* ApplicationArgs::Get(const std::string& arg)
{
    std::string key(arg);
    if (args.find(key) != args.end())
    {
        return args.at(key).c_str();
    }
    return nullptr;
}

std::filesystem::path ApplicationArgs::GetApplicationLocation()
{
    return appPath;
}

ApplicationArgs::ApplicationArgs()
{
    char** argv = g_argv;
    if (argv == nullptr) return;

    appPath = *(argv);
    while (*(++argv))
    {
        if (*(argv)[0] == '-')
        {
            const char* key = *(argv++);
            if (const char* value = *(argv))
            {
                args.emplace(key, value);
            }
        }
    }
}