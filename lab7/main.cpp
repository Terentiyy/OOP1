#include <random>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <array>
#include <iostream>
#include <utility>
#include "NPC.hpp"
#include "Factory.hpp"
#include "Visitor.hpp"

class World {
public:
    World(double size) : worldSize(size) {}
    void AddCharacter(const std::string &type, const std::string &name, float x, float y) {
        if (type == "Dragon") {
            std::shared_ptr<Dragon> dragon = std::make_shared<Dragon>(name, 1, Coordinates(x, y));
            characters.push_back(dragon);
        }
        if (type == "Princess") {
            characters.push_back(std::make_shared<Princess>(name, 1, Coordinates(x, y)));
        }
        if (type == "Knight") {
            characters.push_back(std::make_shared<Knight>(name, 1, Coordinates(x, y)));
        }
    }
    void AnounceFighters() const {
        for (const auto& c : characters) {
            if (!c->isAlive())
                continue;
            Coordinates cor = c->getCoordinates();
            std::cout << c->getType() << " '" << c->getName() << "' ("
                << cor.x << ", " << cor.y << ")\n";
        }
    }

    void setObserver(const std::shared_ptr<IObserver>& observer) {
        std::vector<std::shared_ptr<INPC>> npcs = this->characters;
        for (const auto& c : npcs) {
            c->addObserver(observer);
        }
    }
    void Serialize(const std::string& filename) const {
        std::ofstream fs(filename);
        for (const auto& c : characters) {
            Coordinates cor = c->getCoordinates();
            fs << c->getType() << " " << c->getName() << " "
                << cor.x << " " << cor.y << "\n";
        }
    }
    void Deserialize(const std::string& filename) {
        std::ifstream fs(filename);
        std::string type, name;
        float x, y;
        while (fs >> type >> name >> x >> y) {
            AddCharacter(type, name, x, y);
        }
    }
    void Engage(BattleVisitor& visitor) {
        visitor.battle(characters);
    }
    void MoveCharacters() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng(seed);
        std::uniform_real_distribution dist(-1.0f, 1.0f);

        for (auto c : characters) {
            Coordinates dir = Coordinates{ dist(rng), dist(rng) };
            c->move(dir);

            //c->setCoordinates(Vec2{ std::clamp(c->GetPosX(), 0.0, worldSize), std::clamp(c->GetPosY(), 0.0, worldSize) });
        }
    }
    void PrintMap() const {
        int xSize = int(worldSize);
        int ySize = int(worldSize);

        for (int i = 0; i <= xSize + 2; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';

        for (int i = -ySize; i <= ySize; ++i) {
            std::cout << '|';
            for (int j = -xSize; j <= xSize; ++j) {
                bool printed = false;
                for (const auto& c : characters) {
                    if (!c->isAlive())
                        continue;

                    printed = true;
                    Coordinates cor = c->getCoordinates();
                    if ((int)cor.x== j && (int)cor.y == i) {
                        if (c->getType() == "Knight") {
                            std::cout << 'K';
                            break;
                        }
                        else if (c->getType() == "Dragon") {
                            std::cout << 'D';
                            break;
                        }
                        else if (c->getType() == "Princess") {
                            std::cout << 'P';
                            break;
                        }
                    }
                    else {
                        printed = false;
                    }
                }
                if (!printed) {
                    std::cout << ' ';
                }
            }
            std::cout << '|';
            std::cout << '\n';
        }

        for (int i = 0; i <= xSize + 2; ++i) {
            std::cout << '-';
        }
        std::cout << '\n';
    }
    double GetSize() const {
        return worldSize;
    }
private:
    double worldSize;
    std::vector<std::shared_ptr<INPC>> characters;
};


std::atomic<bool> gameIsRunning{ true };
std::shared_mutex npc_mutex;

int main() {
    World world(50.0);
    BattleVisitor visitor(100.0);
    auto const consoleObserver = std::make_shared<ConsoleObserver>();
    auto const fileObserver = std::make_shared<FileObserver>("../Data/log.txt");



    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::uniform_real_distribution dist(0.0, world.GetSize());

    std::uniform_int_distribution int_dist(0, 2);

    {
        const char* types[] = { "Dragon", "Knight", "Princess" };
        std::array<int, 3> counters = { 0, 0, 0 };
        for (int i = 0; i < world.GetSize() / 3; ++i) {
            const int rand = int_dist(rng);
            std::string type = types[rand];

            world.AddCharacter(type, type + " " + std::to_string(counters[rand]++), dist(rng), dist(rng));
        }
        world.setObserver(consoleObserver);
        world.setObserver(fileObserver);
    }

    std::cout << "Fighters in dungeon:\n";
    std::cout << "------------------------------------------------------------------" << std::endl;
    {
        std::shared_lock lock(npc_mutex);
        world.AnounceFighters();
    }
    std::cout << "------------------------------------------------------------------" << std::endl;

    // moving
    std::thread movement_thread([&]() {
        while (gameIsRunning) {
            {
                std::unique_lock lock(npc_mutex);
                world.MoveCharacters();
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    std::thread battle_thread([&]() {
        while (gameIsRunning) {
            {
                std::unique_lock lock(npc_mutex);
                world.Engage(visitor);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    std::thread display_thread([&]() {
        while (gameIsRunning) {
            {
                std::unique_lock lock(npc_mutex);
                world.PrintMap();
            }
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
        });

    std::this_thread::sleep_for(std::chrono::seconds(30));
    gameIsRunning = false;

    movement_thread.join();
    battle_thread.join();
    display_thread.join();

    std::cout << "\nRemaining Fighters:\n";
    std::cout << "------------------------------------------------------------------" << std::endl;
    {
        std::shared_lock lock(npc_mutex);
        world.PrintMap();
        world.AnounceFighters();
    }
    std::cout << "------------------------------------------------------------------" << std::endl;

    return 0;
}