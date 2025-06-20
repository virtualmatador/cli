#ifndef CLI_CLI_H
#define CLI_CLI_H

#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

class Cli {
public:
  struct Handler {
    std::function<void(std::vector<std::string> &&)> handle_;
    std::size_t arg_min_;
    std::size_t arg_max_;
  };

public:
  Cli(std::vector<std::pair<std::vector<std::string>,
                            std::variant<Cli, Handler>>> &&handlers);
  ~Cli();

private:
  void parse(int &begin, const int &end, const char *argv[]) const;
  void call_handler(const Handler &handler,
                    std::vector<std::string> &args) const;
  const Handler *get_handler(const std::string &command) const;
  const Cli *dive(const std::string &command) const;

private:
  std::vector<std::variant<Cli, Handler>> handlers_;
  std::map<std::string, std::size_t> index_;

public:
  static void
  parse(int argc, const char *argv[],
        std::vector<std::pair<std::vector<std::string>,
                              std::variant<Cli, Handler>>> &&handlers);
};

#endif // CLI_CLI_H
