#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "definitions/datatypes.h"

struct MEMORY {
    byte data[65535]; // 64KiB equivalent to 0xFFFF.

    void load_file(std::string filename);
};