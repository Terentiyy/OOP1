#include "NPC.hpp"
#include <iostream>

std::string NPC::getName() {
    return this->name;
}

std::string NPC::getType() {
    return this->type;
}

int NPC::getHealth() {
    return this->health;
}

Coordinates NPC::getCoordinates() {
    return this->coordinates;
}

void NPC::addObserver(std::shared_ptr<IObserver> observer) {
    this->observers.push_back(observer);
}

bool NPC::isAlive() {
    return this->getHealth() > 0;
}

std::string NPC::status() {
    if (this->isAlive()) {
        std::string status = "Name: " + this->getName() + " Type: " + this->getType() + " Health: " + std::to_string(this->getHealth());
        return status;
    } else {
        std::string status = "Name: " + this->getName() + " Type: " + this->getType() + " Dead";
        return status;
    }
}

std::string NPC::getInfo() {
    std::string status = "Name: " + this->getName() + " Type: " + this->getType() + " Health: " + std::to_string(this->getHealth()) + " X: " + std::to_string(this->getCoordinates().x) + " Y: " + std::to_string(this->getCoordinates().y);
    return status;
}

void NPC::notifyObservers(std::shared_ptr<INPC> other) {
    for (auto &observer : this->observers) {
        std::shared_ptr<INPC> npc = std::make_shared<NPC>(*this);
        observer->logAttack(npc, other);
    }
}

void NPC::notifyObservers() {
    for (auto &observer : this->observers) {
        std::shared_ptr<INPC> npc = std::make_shared<NPC>(*this);
        observer->update(npc);
    }
}



void ConsoleObserver::logAttack(std::shared_ptr<INPC>&  Attacker, std::shared_ptr<INPC>& Defender) {
    std::cout << "Name: " << Attacker->getName() << " Type: " << Attacker->getType() << " attacks " << "Name: " << Defender->getName() << " Type: " << Defender->getType()<< std::endl;
}

void ConsoleObserver::update(std::shared_ptr<INPC>& NPC) {
    std::cout << NPC->status() << std::endl;
}

void FileObserver::logAttack(std::shared_ptr<INPC>& Attacker, std::shared_ptr<INPC>& Defender) {
    if (file.is_open()){
        file << "Name: " << Attacker->getName() << " Type: " << Attacker->getType() << " attacks " << "Name: " << Defender->getName() << " Type: " << Defender->getType()<< std::endl;
    }
    else {
        throw std::runtime_error("File is not open " + path);
    }
}

void FileObserver::update(std::shared_ptr<INPC>& NPC) {
    if (file.is_open()){
        file << NPC->status() << std::endl;
    }
    else {
        throw std::runtime_error("File is not open " + path);
    }
}
