// textfile.cpp
//
// simple reading and writing for text files
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given

#include "textfile.h"

#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

string textFileRead(const string& filename)
{
    ifstream file(filename, ios::binary | ios::ate);

    if (file.is_open())
    {
        const auto size = file.tellg();
        file.seekg(0, ios_base::beg);

        vector<char> content(static_cast<size_t>(size));
        file.read(&content[0], size);

        return string(content.data(), content.size());
    }

    return "";
}

int textFileWrite(const char* fn, const string& text)
{
    ofstream file(fn);

    if (file.is_open())
    {
        file.write(text.c_str(), static_cast<long long>(text.size()));
        return 0;
    }

    return 1;
}
