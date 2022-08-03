#include <cstdint>

namespace detail
{
    // FNV-1a 32bit hashing algorithm.
    constexpr int32_t fnv1a_32(char const* s, size_t count)
    {
        return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
    }
}    // namespace detail

constexpr uint32_t operator"" _hash(char const* s, size_t count)
{
    return detail::fnv1a_32(s, count);
}