#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <experimental/random>
#include "NPC.hpp"

namespace generate {
    std::string inline generateRandamData(){
        std::string alfabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::string name;
        name.resize(7);

        for (int i = 0; i < 7; i++) {
            name[i] = alfabet[(rand() % alfabet.size())];
        }

        int x = std::experimental::randint(0, 500);
        int y = std::experimental::randint(0, 500);
        std::string data = name + " " + "1 " +std::to_string(x) + " " + std::to_string(y);
        return data;
    }

    void inline generateInputFile(const std::string& path, int const size) {
        std::ofstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("File is not open " + path);
        }
        for (int i = 0; i < size; i++) {
            file << generateRandamData() << "\n";
        }
        file.close();
    }
}

class IFactory{
    public:
        virtual std::shared_ptr<INPC> creatNpc() = 0;

    protected:
        virtual ~IFactory() = default;
};


class DragonFactory final: public IFactory {
    public:
        DragonFactory(std::string path) : path(std::move(path)) {};
        std::shared_ptr<INPC> creatNpc() override;

    private:
        std::string path;

};

class KnightFactory final: public IFactory {
    public:
        KnightFactory(std::string path) : path(std::move(path)) {};
        std::shared_ptr<INPC> creatNpc() override;

    private:
        std::string path;

};

class PrincessFactory final: public IFactory {
    public:
        PrincessFactory(std::string path) : path(std::move(path)) {};
        std::shared_ptr<INPC> creatNpc() override;

    private:
        std::string path;

};