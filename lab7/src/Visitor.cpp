#include "Visitor.hpp"


bool BattleVisitor::isInRange(const std::shared_ptr<INPC>& npc1, const std::shared_ptr<INPC>& npc2) const{
    double limitation = npc1->getAttackDistance();
    limitation *= limitation;
    float delX = npc1->getCoordinates().x - npc2->getCoordinates().x;
    float delY = npc1->getCoordinates().y - npc2->getCoordinates().y;
    float distance = delX * delX + delY * delY;
    return distance <= limitation;
}

bool BattleVisitor::canFight(std::shared_ptr<INPC>& npc1, std::shared_ptr<INPC>& npc2) const{
    if (npc1->getType() == "Dragon" && npc2->getType() == "Princess") return true;
    if (npc1->getType() == "Knight" && npc2->getType() == "Dragon") return true;
    return false;
}

std::pair<bool, bool> BattleVisitor::outcomeOfTheBattle(std::shared_ptr<INPC>& npc1, std::shared_ptr<INPC>& npc2) {
    int lucky1 = rand() % 6 + 1;
    int lucky2 = rand() % 6 + 1;
    bool lucky = lucky1 < lucky2;

    return {true, lucky};
}


void BattleVisitor::battle(std::vector<std::shared_ptr<INPC>>& npcs) {

    for (size_t i = 0; i < npcs.size(); ++i) {
        if (!npcs[i]->isAlive()) {
            npcs.erase(npcs.begin() + i);
        }
    }

    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            if (isInRange(npcs[i], npcs[j]) && canFight(npcs[i], npcs[j])) {
                std::pair<bool, bool> outcome = outcomeOfTheBattle(npcs[i], npcs[j]);
                if (!outcome.first) {
                    npcs[j]->notifyObservers(npcs[i]);
                    npcs[i]->damage(npcs[i]->getHealth());
                    npcs[i]->notifyObservers();
                    npcs.erase(npcs.begin() + i);

                }
                if (!outcome.second) {
                    npcs[i]->notifyObservers(npcs[j]);
                    npcs[j]->damage(npcs[j]->getHealth());
                    npcs[j]->notifyObservers();
                    npcs.erase(npcs.begin() + j);
                }
            }
        }
    }
}