#pragma once

class flags
{
    flags() = default;
public:
    static flags& get()
    {
        static flags inst;
        return inst;
    }

    int verbose_level() const { return _verbose; }

    bool is_show_stats() const { return _show_stats; }

    bool is_omit_zero() const { return _omit_zero; }

    bool use_relative_path() const { return _relative_path; }

    friend class options;

private:
    int _verbose{ 0 };
    bool _show_stats{ false };
    bool _omit_zero{ false };
    bool _relative_path{ false };
};
