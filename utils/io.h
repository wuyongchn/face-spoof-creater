#ifndef SPOOF_CREATER_UTILS_IO_H_
#define SPOOF_CREATER_UTILS_IO_H_

#include <fstream>
#include <string>
#include <vector>

bool WriteFile(const std::string& filename, std::vector<unsigned char>& buffer);

bool ReadFile(const std::string& filename, std::vector<unsigned char>& buffer);

#endif  // SPOOF_CREATER_UTILS_IO_H_
