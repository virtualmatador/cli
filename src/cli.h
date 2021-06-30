#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

class Cli
{
public:
    struct Handler
    {
        std::function<void(const std::vector<std::string>&)> handle_;
        std::size_t arg_max_;
    };

public:
    Cli(std::map<std::string, std::variant<Cli, Handler>>&& handlers);
    ~Cli();

private:
    void parse(int& begin, const int& end, const char* argv[]) const;
    const Handler* get_handler(const std::string& command) const;
    const Cli* dive(const std::string& command) const;

private:
    const std::map<std::string, std::variant<Cli, Handler>> handlers_;

public:
    static void parse(int argc, const char* argv[],
        std::map<std::string, std::variant<Cli, Handler>>&& handlers);
};
