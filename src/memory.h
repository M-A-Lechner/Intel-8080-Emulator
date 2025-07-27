#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <array>

#include "definitions/datatypes.h"

struct MEMORY {
    std::array<little_byte, 65535> data; // 64KiB equivalent to 0xFFFF.

    void reset();

    void load_file(std::string filename);
};