#include <iostream>

#include <cli.h>

int main()
{
    int result = 0;
    try
    {
        const char* argv[] =
        {
            "test",
            "--version",
        };
        Cli::parse(sizeof(argv) / sizeof(*argv), argv,
        {
            {
                "--version", Cli::Handler({ [&](const std::vector<std::string>&)
                {
                    result += 1;
                }, 0})
            },
            {
                "task", Cli({
                {
                    "--version", Cli::Handler({ [&](const std::vector<std::string>&)
                    {
                        result += 2;
                    }, 0})
                }})
            }
        });
    }
    catch(const std::exception& e)
    {
        result = -1;
        std::cerr << e.what() << std::endl;
    }
    if (result == 1)
    {
        return 0;
    }
    return -1;
}
