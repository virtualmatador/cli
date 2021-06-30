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
            "123",
        };
        Cli::parse(sizeof(argv) / sizeof(*argv), argv,
        {
            {
                "--version", Cli::Handler({ [&](const std::vector<std::string>&)
                {
                    result += 1;
                }, 0})
            }
        });
    }
    catch(const std::exception& e)
    {
        result += 2;
        std::cerr << e.what() << std::endl;
    }
    if (result == 3)
    {
        return 0;
    }
    return -1;
}
