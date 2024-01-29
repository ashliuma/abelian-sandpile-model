#include "bmp.h"


Image::Image(uint16_t width, uint16_t height) {
    m_height = height;
    m_width = width;
    m_colors = new Color[width * height];
}

void Image::SetColor(const Color &color, uint16_t x, uint16_t y) const {
    m_colors[m_width * y + x].r = color.r;
    m_colors[m_width * y + x].g = color.g;
    m_colors[m_width * y + x].b = color.b;
}

bool operator ==(Color& a, Color& b){
    return a.r == b.r && a.g == b.g && a.b == b.b;
}

uint8_t Image::GetColor(uint16_t x, uint16_t y, Color*& kColors) const {
    Color col = m_colors[m_width * y + x];
    for(uint8_t i = 0; i < 5; ++i){
        if (col == kColors[i])
            return i;
    }
    return 0;
}

void Image::ExportToBMP(const std::filesystem::path& path) const {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Couldn't open this file";
        exit(1);
    }
    const uint16_t kPaddingAmount = (8 - (m_height) % 8) % 8;
    const uint64_t kFullLength = m_height + kPaddingAmount;
    const uint64_t kFileHeaderSize = 14;
    const uint64_t kInformHeaderSize = 40;
    const uint64_t kpaletteSize = 20;
    const uint64_t kFileSize = kFileHeaderSize + kInformHeaderSize + kpaletteSize + (kFullLength * m_width / 2);
    const uint16_t kBitsPerPixel = 4;
    const uint16_t kTotalColors = 16;

    uint8_t file_header[kFileHeaderSize];
    //File type
    file_header[0] = 'B';
    file_header[1] = 'M';
    //File size in bytes
    file_header[2] = kFileSize;
    file_header[3] = kFileSize >> 8;
    file_header[4] = kFileSize >> 16;
    file_header[5] = kFileSize >> 24;
    //Reserved (not used)
    for (int i = 6; i < 10; i++) {
        file_header[i] = 0;
    }
    //Pixel data offset (size between start pixels data and start)
    file_header[10] = kFileHeaderSize + kInformHeaderSize + kpaletteSize;
    file_header[11] = 0;
    file_header[12] = 0;
    file_header[13] = 0;

    uint8_t DIB_header[kInformHeaderSize];
    // Header size
    DIB_header[0] = kInformHeaderSize;
    DIB_header[1] = 0;
    DIB_header[2] = 0;
    DIB_header[3] = 0;
    // Image width
    DIB_header[4] = m_width;
    DIB_header[5] = m_width >> 8;
    DIB_header[6] = m_width >> 16;
    DIB_header[7] = m_width >> 24;
    // Image height. Positive for bottom to top pixel order
    DIB_header[8] = m_height;
    DIB_header[9] = m_height >> 8;
    DIB_header[10] = m_height >> 16;
    DIB_header[11] = m_height >> 24;
    // Color planes (always 1 for bmp)
    DIB_header[12] = 1;
    DIB_header[13] = 0;
    // Bits per pixel (RGB)
    DIB_header[14] = kBitsPerPixel;
    DIB_header[15] = 0;
    // 16-19 Compression (no compression)
    // 20-23 Image size (no compression)
    // 24-27 X pixels per meter (not specified)
    // 28-31 Y pixels per meter (not specified)
    // 32-35 Color palette
    // 36-39 Important colors (ignored)
    for (int i = 16; i < 32; i++) {
        DIB_header[i] = 0;
    }
    DIB_header[32] = kTotalColors;
    for (int i = 33; i < kInformHeaderSize; i++) {
        DIB_header[i] = 0;
    }

    uint8_t palette[kpaletteSize];
    Color *kColors = new Color[5];
    kColors[0] = {255, 255, 255};
    kColors[1] = {0, 255, 0};
    kColors[2] = {255, 255, 0};
    kColors[3] = {255, 0, 255};
    kColors[4] = {0, 0, 0};

    palette[0] = kColors[0].b;
    palette[1] = kColors[0].g;
    palette[2] = kColors[0].r;
    palette[3] = 0;
    palette[4] = kColors[1].b;
    palette[5] = kColors[1].g;
    palette[6] = kColors[1].r;
    palette[7] = 0;
    palette[8] = kColors[2].b;
    palette[9] = kColors[2].g;
    palette[10] = kColors[2].r;
    palette[11] = 0;
    palette[12] = kColors[3].b;
    palette[13] = kColors[3].g;
    palette[14] = kColors[3].r;
    palette[15] = 0;
    palette[16] = kColors[4].b;
    palette[17] = kColors[4].g;
    palette[18] = kColors[4].r;
    palette[19] = 0;
    file.write(reinterpret_cast<char *>(file_header), kFileHeaderSize);
    file.write(reinterpret_cast<char *>(DIB_header), kInformHeaderSize);
    file.write(reinterpret_cast<char *>(palette), kpaletteSize);
    uint8_t first_color;
    uint8_t second_color;
    uint8_t end_color;
    for (uint16_t y = 0; y < m_height; y++) {
        for (uint16_t x = 0; x < kFullLength; x += 2) {
            first_color = GetColor(x, y, kColors);
            second_color = GetColor(x + 1, y, kColors);

            end_color = (first_color << 4) + second_color;
            file << (end_color);
        }
    }
}
