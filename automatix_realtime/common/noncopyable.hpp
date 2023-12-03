#pragma once

namespace amx
{
    class noncopyable
    {
    public:
        noncopyable() {}
        ~noncopyable() {}

        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
}


