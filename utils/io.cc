#include "utils/io.h"

bool WriteFile(const std::string& filename,
               std::vector<unsigned char>& buffer) {
  std::fstream file(filename.c_str(),
                    std::ios::out | std::ios::binary | std::ios::ate);
  if (file.is_open()) {
    file.write(reinterpret_cast<char*>(&buffer[0]), buffer.size());
    file.close();
    return true;
  } else {
    return false;
  }
}

bool ReadFile(const std::string& filename, std::vector<unsigned char>& buffer) {
  std::streampos size;
  std::fstream file(filename.c_str(),
                    std::ios::in | std::ios::binary | std::ios::ate);
  if (file.is_open()) {
    size = file.tellg();
    buffer.resize(size);
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&buffer[0]), size);
    file.close();
    return true;
  } else {
    return false;
  }
}
