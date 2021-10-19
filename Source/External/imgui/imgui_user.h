#pragma once
#include <vector>
#include <string>

inline bool VectorStringGetter(void* data, int n, const char** out_text)
{
    const std::vector<std::string>* v = (std::vector<std::string>*)data;
    *out_text                         = (*v)[n].c_str();
    return true;
}
