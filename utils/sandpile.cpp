#include "sandpile.h"
#include "bmp.cpp"


void Sandpile::GetValues(const ArgParser::Arguments& arglist) {
    tsv_path = arglist.infile;
    out_path = arglist.output;
    max_iter = arglist.max_iter;
    freq = arglist.freq;
}

void Sandpile::FromTSV(std::filesystem::path& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Couldn't open this file";
        exit(1);
    }
    if (out_path.string().back() != '/') {
        out_path += "/";
    }

    max_x = length;
    max_y = width;
    sandpile.add(max_x, max_y);

    std::string str;
    while (getline(file, str)) {
        uint16_t x = static_cast<uint16_t>(std::stoi(str.substr(0, str.find(' '))));
        uint16_t y = static_cast<int16_t>(std::stoi(str.substr(str.find('\t') + 1, str.rfind('\t'))));
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
        uint64_t value = std::stoull(str.substr(str.rfind(' '), str.size()));
        SetGrains({x, y}, value);
    }
}

uint32_t Sandpile::Index(std::pair<uint16_t, uint16_t> coord){
    uint32_t result = static_cast<uint32_t>(coord.second * (UINT16_MAX + 1) + coord.first);
    return result;
}

void Sandpile::SetGrains(std::pair<uint16_t, uint16_t> coord, uint64_t value) {
    sandpile.add(value, Index(coord));
    if (value >= 4) {
        queue.push(coord);
    }
    max_x = std::max(max_x, coord.first);
    min_x = std::min(min_x, coord.first);
    max_y = std::max(max_y, coord.second);
    min_y = std::min(min_y, coord.second);
}

void Sandpile::Spilling() {
    size_t old_size = queue.size();
    uint64_t iter = 0;
    while (!queue.empty()) {
        if (iter == max_iter) {
            break;
        }

        uint16_t x = queue.front().first;
        uint16_t y = queue.front().second;
        queue.pop();
        if (sandpile.grid[Index({x, y})] < 4) {
            continue;
        }
        uint16_t step = 1;
        std::pair<uint16_t, uint16_t> left = {x - step, y};
        std::pair<uint16_t, uint16_t> right = {x + step, y};
        std::pair<uint16_t, uint16_t> up = {x, y + step};
        std::pair<uint16_t, uint16_t> down = {x, y - step};

        max_x = std::max(max_x, right.first);
        min_x = std::min(min_x, left.first);
        max_y = std::max(max_y, up.second);
        min_y = std::min(min_y, down.second);

        std::pair<uint16_t , uint16_t> coords[] = {up, down, left, right};

        for (auto& coord : coords) {
            if(Index(coord) >= sandpile.size()){
                sandpile.add(0, Index(coord));
            }
            sandpile.grid[Index(coord)]++;
            sandpile.grid[Index({x, y})]--;

            if (sandpile.grid[Index(coord)] >= 4) {
                queue.push(coord);
            }
        }

        if (sandpile.grid[Index({x,y})] >= 4) {
            queue.push({x, y});
        }

        old_size--;
        max_x = std::max(max_x, right.first);
        min_x = std::min(min_x, left.first);
        max_y = std::max(max_y, up.second);
        min_y = std::min(min_y, down.second);
        if (old_size == 0) {
            old_size = queue.size();
            iter++;
            if (freq != 0 and iter % freq == 0) {
                CreateBMP(freq == 0);
            }
        }
    }
    CreateBMP(freq == 0);
}

void Sandpile::CreateBMP(bool freqZero) {
    uint16_t widthBMP = max_x - min_x + 1;
    uint16_t heightBMP = max_y - min_y + 1;
    Image image(widthBMP, heightBMP);
    const Color colors[] = {{255, 255, 255}, {0, 255, 0}, {255, 255, 0}, {255, 0, 255}, {0, 0, 0}};

    for (uint16_t x = min_x; x <= max_x; ++x){
        for (uint16_t y = min_y; y <= max_y; ++y){
            if (sandpile.grid[Index({x, y})] < 4) {
                image.SetColor(colors[sandpile.grid[Index({x,y})]], x - min_x, y - min_y);
                //for(uint16_t p = 0; p < widthBMP * heightBMP; p++){
                //    std::cout << int(image.m_colors[p].r) << ' ' << int(image.m_colors[p].g) << ' ' << int(image.m_colors[p].b) << '\n';
                //}
            }
            else {
                image.SetColor(colors[4], x - min_x, y - min_y);
            }
        }
    }

    std::filesystem::path filename = out_path;
    filename += "sandpile";
    if (!freqZero) {
        filename += '_' + std::to_string(version);
        version++;
    }
    filename += ".bmp";

    image.ExportToBMP(filename);
    delete[] image.m_colors;
}
