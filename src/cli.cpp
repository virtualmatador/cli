#include <stdexcept>
#include <cstring>

#include "cli.h"

Cli::Cli(std::map<std::string, std::variant<Cli, Cli::Handler>>&& handlers)
    : handlers_{ std::move(handlers) }
{
}

Cli::~Cli()
{
}

void Cli::parse(int& progress, const int& end, const char* argv[]) const
{
    const Cli* cli = nullptr;
    const Handler* handler = nullptr;
    std::vector<std::string> args;
    const int old_progress = progress;
    for (; progress < end; ++progress)
    {
        if (std::strlen(argv[progress]) == 0)
        {
            continue;
        }
        if (argv[progress][0] == '-')
        {
            if (handler)
            {
                call_handler(*handler, args);
            }
            if (std::strlen(argv[progress]) <= 1 || argv[progress][1] != '-')
            {
                handler = get_handler('-' + std::string(1, argv[progress][1]));
                for (std::size_t i = 2; i < std::strlen(argv[progress]); ++i)
                {
                    call_handler(*handler, args);
                    handler = get_handler('-' + std::string(1, argv[progress][i]));
                }
            }
            else if (std::strlen(argv[progress]) == 2)
            {
                ++progress;
                handler = nullptr;
                break;
            }
            else
            {
                handler = get_handler(argv[progress]);
            }
        }
        else if (progress == old_progress && (cli = dive(argv[progress])))
        {
            cli->parse(++progress, end, argv);
            --progress;
        }
        else
        {
            if (!handler)
            {
                handler = get_handler("");
            }
            else if (handler->arg_max_ == args.size())
            {
                call_handler(*handler, args);
                handler = get_handler("");
            }
            args.push_back(argv[progress]);
        }
    }
    if (handler)
    {
        call_handler(*handler, args);
        handler = nullptr;
    }
}

void Cli::call_handler(
    const Handler& handler, std::vector<std::string>& args) const
{
    if (args.size() >= handler.arg_min_)
    {
        handler.handle_(args);
        args.clear();
    }
    else
    {
        throw std::runtime_error("Too Few Arguments");
    }
}


const Cli::Handler* Cli::get_handler(const std::string& command) const
{
    auto handler = handlers_.find(command);
    if (handler != handlers_.end() && handler->second.index() == 1)
    {
        return &std::get<1>(handler->second);
    }
    else
    {
        if (command.empty())
        {
            throw std::runtime_error("Unwanted Argument");
        }
        else
        {
            throw std::runtime_error("Bad Switch: " + command);
        }
        return nullptr;
    }
}

const Cli* Cli::dive(const std::string& command) const
{
    auto handler = handlers_.find(command);
    if (handler != handlers_.end() && handler->second.index() == 0)
    {
        return &std::get<0>(handler->second);
    }
    else
    {
        return nullptr;
    }
}

void Cli::parse(int argc, const char* argv[],
    std::map<std::string, std::variant<Cli, Handler>>&& handlers)
{
    int progress = 1;
    Cli cli(std::move(handlers));
    cli.parse(progress, argc, argv);
}
