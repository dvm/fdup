#pragma once

template <typename In, typename Out>
Out const& operator|(In const& in, Out& out)
{
    return out.exec(in);
}