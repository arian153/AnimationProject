#pragma once
#include <iostream>

namespace CS460
{
    namespace Console
    {
        void CreateConsole();
        void DestroyConsole();
        void ClearConsole();
        void MessageOut(const std::string& output_message);
        void MessageOut(const std::wstring& output_message);

        template <typename First, typename... Rest>
        void ConsoleOut(const First& first, const Rest&...rest)
        {
            std::cout << first << ", ";
            ConsoleOut(rest...);
        }

        template <typename T>
        void ConsoleOut(const T& t)
        {
            std::cout << t;
        }

        inline void ConsoleOut()
        {
        }
    }
}
