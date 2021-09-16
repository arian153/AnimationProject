
namespace CS460
{
    template <typename First, typename... Rest>
    void UnusedParameter(const First& first, const Rest& ... rest)
    {
        first;
        UnusedParameter(rest...);
    }

    template <typename T>
    void UnusedParameter(const T& t)
    {
        t;
    }
}
