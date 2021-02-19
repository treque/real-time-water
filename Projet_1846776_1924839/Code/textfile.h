#ifndef TEXT_FILE_H
#define TEXT_FILE_H

#include <string>
#include <vector>

// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given

// Prototypes des fonctions
std::string textFileRead(const std::string& filename);
int               textFileWrite(const char* fn, const std::string& text);

#endif
