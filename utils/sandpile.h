#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <algorithm>
#include "ArgParser.h"


class Grid {

private:
    uint32_t capacity;

public:
    uint64_t * grid;

    Grid() {
        grid = new uint64_t[1];
        grid[0] = 1;
        capacity = 1;
    }

    ~Grid() {
        delete [] grid;
    }

    [[nodiscard]] uint32_t size() const {
        return capacity;
    }

    void add(uint64_t data, uint32_t index) {
        if (index >= capacity){
            uint64_t* temp = new uint64_t[std::max(2 * capacity, index + 1)] {};

            for (uint32_t i = 0; i < capacity; ++i) {
                temp[i] = grid[i];
            }

            delete[] grid;
            capacity = std::max(2 * capacity, index + 1);
            grid = temp;
        }
        grid[index] = data;
    }
};

class Queue {

private:
    struct Node {
        std::pair<uint16_t, uint16_t> coord;
        Node* next = nullptr;
        explicit Node(std::pair<uint16_t, uint16_t>& _coord) : coord(_coord), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    size_t qsize;

public:
    Queue() : head(nullptr), tail(nullptr), qsize(0) {}

    ~Queue() {
        Node *temp = head;
        while (temp != nullptr) {
            temp = head->next;
            delete head;
            head = temp;
        }
    }

    bool empty() {
        return head == nullptr;
    }

    [[nodiscard]] size_t size() const{
        return qsize;
    }

    void push(std::pair<uint16_t, uint16_t> coord) {
        Node *v = new Node(coord);
        qsize++;
        if (empty()) {
            head = v;
            tail = v;
        } else {
            tail->next = v;
            tail = v;
        }
    }

    std::pair<uint16_t, uint16_t> front(){
        return head->coord;
    }

    void pop() {
        if (empty()){
            return;
        }
        Node *p = head;
        qsize--;
        head = head->next;
        delete p;
    }
};

class Sandpile {

private:
    uint16_t max_x = 0;
    uint16_t min_x = UINT16_MAX;
    uint16_t max_y = 0;
    uint16_t min_y = UINT16_MAX;
    uint64_t version = 1;
    Queue queue;
    Grid sandpile;

public:
    std::filesystem::path tsv_path;
    std::filesystem::path out_path = "./";
    uint16_t length = 100;
    uint16_t width = 100;
    uint64_t max_iter = INT64_MAX;
    uint64_t freq = 0;

    void GetValues(const ArgParser::Arguments& arglist);

    void FromTSV(std::filesystem::path& file_path);

    static uint32_t Index(std::pair<uint16_t, uint16_t> coord);

    void SetGrains(std::pair<uint16_t, uint16_t> coord, uint64_t value);

    void Spilling();

    void CreateBMP(bool freq_zero);
};
