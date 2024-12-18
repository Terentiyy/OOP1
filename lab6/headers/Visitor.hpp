#pragma once
#include "NPC.hpp"


class IVisitor {
    public:
        virtual void battle(std::vector<std::shared_ptr<INPC>>& npcs) = 0;
        virtual ~IVisitor() = default;
};

class BattleVisitor : public IVisitor {
    public:
        BattleVisitor(float range) : range(range) {};
        void battle(std::vector<std::shared_ptr<INPC>>& npcs) override;

    private:
        bool isInRange(std::shared_ptr<INPC>& npc1, std::shared_ptr<INPC>& npc2) const;
        bool canFight(std::shared_ptr<INPC>& npc1, std::shared_ptr<INPC>& npc2) const;
        std::pair<bool, bool> outcomeOfTheBattle(std::shared_ptr<INPC>& npc1, std::shared_ptr<INPC>& npc2);


    private:
        float range;
};