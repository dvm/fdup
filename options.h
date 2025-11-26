#pragma once

#include <set>
#include <filesystem>

class options
{
public:
    using result_type = std::set<std::filesystem::path>;

    options() = delete;
    options(int argc, char** argv);

    void parse();

    void exit_usage(int exit_code) const;

    result_type const& result() const
    {
        return _directories;
    }

private:
    int _argc;
    char** _argv;
    std::string _progname;

    result_type _directories{};
};
