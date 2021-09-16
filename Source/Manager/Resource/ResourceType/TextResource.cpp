#include "TextResource.hpp"
#include <fstream>

namespace CS460
{
    TextResource::TextResource(const std::wstring& path)
        : Resource(path)
    {
    }

    TextResource::~TextResource()
    {
    }

    void TextResource::Initialize()
    {
        m_b_loaded = LoadText();
    }

    void TextResource::Shutdown()
    {
        m_b_unloaded = true;
    }

    bool TextResource::LoadText()
    {
        std::wifstream file;
        file.open(m_file_path_w);
        if (file)
        {
            m_text.clear();
            file.seekg(0, file.end);
            size_t length = file.tellg();
            file.seekg(0, file.beg);
            m_text.reserve(length);
            //read
            while (file.eof() == false)
            {
                std::wstring line;
                std::getline(file, line);
                m_text.append(line + L"\n");
            }
            file.close();
            return true;
        }
        return false;
    }

    std::wstring TextResource::GetText() const
    {
        return m_text;
    }
}
