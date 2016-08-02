#ifndef _NONE_COPYABLE_H_
#define _NONE_COPYABLE_H_
class NonCopyable
{
protected:
    NoneCopyable() = default;
    ~NoneCopyable() = default;
    NoneCopyable(const NoneCopyable&) = delete; // disable copy construction
    NoneCopyable& operator = (const NoneCopyable&) = delete; // disable assign operation
};
#endif
