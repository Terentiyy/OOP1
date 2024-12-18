#include "Factory.hpp"

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <random>


#define range 10

std::vector<std::string> get_info(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("File is not open " + path);
    }
    std::vector<std::string> info;
    std::string line;
    while (std::getline(file, line)) {
        info.push_back(line);
    }
    file.close();
    size_t index = rand() % info.size();
    line = info[index];

    std::stringstream ss(line);
    std::string name,health, x, y;
    ss >> name >> health >> x >> y;

    return {name, health, x, y};
}

bool InMap(const Coordinates& coordinates) {
    const bool rul = coordinates.x > -range && coordinates.y > -range && coordinates.x < range && coordinates.y < range;
    return rul;

}




std::shared_ptr<INPC> DragonFactory::creatNpc() {
    std::vector<std::string> info = get_info(this->path);
    if (!InMap(Coordinates(std::stof(info[2]), std::stof(info[3])))) {
        return std::make_shared<Dragon>(info[0], std::stoi(info[1]), Coordinates(0, 0));
    }
    return std::make_shared<Dragon>(info[0], std::stoi(info[1]), Coordinates(std::stof(info[2]), std::stof(info[3])));

}

std::shared_ptr<INPC> KnightFactory::creatNpc() {
    std::vector<std::string> info = get_info(this->path);
    if (!InMap(Coordinates(std::stof(info[2]), std::stof(info[3])))) {
        return std::make_shared<Knight>(info[0], std::stoi(info[1]), Coordinates(0, 0));
    }
    return std::make_shared<Knight>(info[0], std::stoi(info[1]), Coordinates(std::stof(info[2]), std::stof(info[3])));

}

std::shared_ptr<INPC> PrincessFactory::creatNpc() {
    std::vector<std::string> info = get_info(this->path);
    if (!InMap(Coordinates(std::stof(info[2]), std::stof(info[3])))) {
        return std::make_shared<Princess>(info[0], std::stoi(info[1]), Coordinates(0, 0));
    }
    return std::make_shared<Princess>(info[0], std::stoi(info[1]), Coordinates(std::stof(info[2]), std::stof(info[3])));

}