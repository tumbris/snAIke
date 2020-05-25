#include <snAIke/Application/Application.hpp>

char** g_argv = nullptr;

int main(int argc, char** argv)
{
    g_argv = argv;

    Application app;
    app.Init(800, 600, "snAIke");
    return app.Run();
}