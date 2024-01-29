#pragma once

#include <iostream>
#include <fstream>
#include <cinttypes>
#include <filesystem>
#include "sandpile.h"


struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


class Image {
public:
    Color* m_colors;
    Image(uint16_t width, uint16_t height);
    ~Image() = default;

    [[nodiscard]] uint8_t GetColor(uint16_t x, uint16_t y, Color*& kColors) const;

    void SetColor(const Color& color, uint16_t x, uint16_t y) const;

    void ExportToBMP(const std::filesystem::path& path) const;

private:
    uint16_t m_width;
    uint16_t m_height;

};