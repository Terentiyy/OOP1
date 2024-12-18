#include "NPC.hpp"
#include <iostream>
#include <cmath>

std::string NPC::getName() const {
    return this->name;
}

std::string NPC::getType() const {
    return this->type;
}

int NPC::getHealth() const {
    return this->health;
}

Coordinates NPC::getCoordinates() const {
    return this->coordinates;
}

void NPC::addObserver(std::shared_ptr<IObserver> observer) {
    this->observers.push_back(observer);
}

bool NPC::isAlive() {
    return this->getHealth() > 0;
}







void NPC::setCoordinates(const Coordinates coordinates) {
    auto [x, y] = getCoordinates();
    const bool rul2 = pow(coordinates.x - x, 2) + pow(coordinates.y - y, 2) <= pow(this->getMoveDistance(), 2);
    if (rul2) this->coordinates = coordinates;
}


std::string NPC::status() {
    if (this->isAlive()) {
        std::string status = this->getInfo();
        return status;
    } else {
        std::string status = "Name: " + this->getName() + " Type: " + this->getType() + " Dead";
        return status;
    }
}

std::string NPC::getInfo() const {
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

void NPC::move(const Coordinates& coordinates) {

    if (canMove(coordinates)) {
        this->notifyObservers();
        this->setCoordinates(coordinates);
        this->notifyObservers();

    }
    else {
        Coordinates oldCoordinates = this->getCoordinates();
        Coordinates newCoordinates = {(oldCoordinates.x + coordinates.x)/2, (oldCoordinates.y + coordinates.y)/2};
        move(newCoordinates);
    }

}


bool NPC::canMove(const Coordinates& coordinates) {
    double limitation = this->getMoveDistance();
    limitation *= limitation;
    float delX = this->getCoordinates().x - coordinates.x;
    float delY = this->getCoordinates().y - coordinates.y;
    float distance = delX * delX + delY * delY;
    if (distance <= limitation) return true;
    return false;
}