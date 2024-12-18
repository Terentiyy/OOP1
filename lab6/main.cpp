#include <iostream>
#include "NPC.hpp"
#include "Factory.hpp"
#include "string"
#include <Visitor.hpp>


int main() {
    generate::generateInputFile("../Data/test.txt", 100);

    DragonFactory dragonFactory("../Data/test.txt");
    PrincessFactory princessFactory("../Data/test.txt");
    KnightFactory knightFactory("../Data/test.txt");

    std::vector<std::shared_ptr<INPC>> npcs;
    auto const consoleObserver = std::make_shared<ConsoleObserver>();
    auto const fileObserver = std::make_shared<FileObserver>("../Data/log.txt");

    for (int i = 0; i < 5; ++i) {
        auto dragon = dragonFactory.creatNpc();
        auto princess = princessFactory.creatNpc();
        auto knight = knightFactory.creatNpc();

        dragon->addObserver(consoleObserver);
        dragon->addObserver(fileObserver);
        princess->addObserver(consoleObserver);
        princess->addObserver(fileObserver);
        knight->addObserver(consoleObserver);
        knight->addObserver(fileObserver);

        npcs.push_back(std::move(dragon));
        npcs.push_back(std::move(princess));
        npcs.push_back(std::move(knight));
    }
    std::cout << "=== List of created NPCs ===" << std::endl;
    for (auto const& npc : npcs) {
        std::cout << npc->getInfo() << std::endl;
    }

    std::cout << "=== Battle ===" << std::endl;
    BattleVisitor visitor(300.0f);
    visitor.battle(npcs);

    std::cout << "=== List of surviving NPCs after the battle ===" << std::endl;
    for (auto const& npc : npcs) {
        std::cout << npc->getInfo() << std::endl;
    }
    return 0;
}