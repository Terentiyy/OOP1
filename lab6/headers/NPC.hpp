#pragma once

#include <string>
#include <vector>
#include <memory>
#include <fstream>

class INPC;

class IObserver {
public:
    virtual void update(std::shared_ptr<INPC>& NPC) = 0;
    virtual void logAttack(std::shared_ptr<INPC>& Attacker, std::shared_ptr<INPC>& Defender) = 0;
    virtual ~IObserver() = default;
};

class ConsoleObserver final: public IObserver {
    public:
        ConsoleObserver() = default;
        void update(std::shared_ptr<INPC>& NPC) override;
        void logAttack(std::shared_ptr<INPC>& Attacker, std::shared_ptr<INPC>& Defender) override;
};

class FileObserver final: public IObserver {
    public:
        explicit FileObserver(std::string path) : path(path) {
            file.open(path);
            if (!file.is_open()) throw std::runtime_error("File is not open " + path);
        }

        void update(std::shared_ptr<INPC>& NPC) override;
        void logAttack(std::shared_ptr<INPC>& Attacker, std::shared_ptr<INPC>& Defender) override;

        ~FileObserver() override{
            if (file.is_open()) file.close();
        }

private:
    std::string path;
    std::ofstream file;
};

struct Coordinates {
    float x;
    float y;
};

class INPC : public std::enable_shared_from_this<INPC>{
    public:
        virtual std::string getName() = 0;
        virtual std::string getType() = 0;
        virtual int getHealth() = 0;
        virtual Coordinates getCoordinates() = 0;
        virtual std::string status() = 0;
        virtual std::string getInfo() = 0;

        virtual void damage(int damage) = 0;

        virtual void addObserver(std::shared_ptr<IObserver> observer) = 0;
        virtual void notifyObservers(std::shared_ptr<INPC> other) = 0;
        virtual void notifyObservers() = 0;
        virtual bool isAlive() = 0;

    protected:
        virtual ~INPC() = default;
};

class NPC : public INPC {
    public:

        NPC(std::string name, std::string type, const int health, const Coordinates coordinates) :
    name(std::move(name)), type(std::move(type)), health(health){
                if (coordinates.x > 0 && coordinates.y > 0 && coordinates.x < 501 && coordinates.y < 501) this->coordinates = coordinates;
                else this->coordinates = {22, 22};
    };

        std::string getName() override;
        std::string getType() override;
        int getHealth() override;
        Coordinates getCoordinates() override;

        void damage(const int damage) override {
            health -= damage;
        }

        void addObserver(std::shared_ptr<IObserver> observer) override;
        void notifyObservers(std::shared_ptr<INPC> other) override;
        void notifyObservers() override;
        bool isAlive() override;
        std::string status() override;
        std::string getInfo() override;

        ~NPC() override = default;


    private:
        std::string name;
        std::string type;
        int health;
        Coordinates coordinates;
        std::vector<std::shared_ptr<IObserver>> observers = {};
};

class Dragon final: public NPC {
    public:
        Dragon(std::string name, const int health, const Coordinates coordinates) :
    NPC(std::move(name), "Dragon", health, coordinates) {};
};

class Knight final: public NPC {
    public:
        Knight(std::string name, const int health, const Coordinates coordinates) :
    NPC(std::move(name), "Knight", health, coordinates) {};
};

class Princess final: public NPC {
    public:
        Princess(std::string name, const int health, const Coordinates coordinates) :
    NPC(std::move(name), "Princess", health, coordinates) {};
};