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

struct NPCDistance {
    double moveDistance;
    double attackDistance;
};

class INPC : public std::enable_shared_from_this<INPC>{
    public:
        virtual std::string getName() const = 0;
        virtual std::string getType() const = 0;
        virtual int getHealth() const = 0;
        virtual Coordinates getCoordinates() const = 0;
        virtual std::string status() = 0;
        virtual std::string getInfo() const = 0;

        virtual double getMoveDistance() const = 0;
        virtual double getAttackDistance() const = 0;

        virtual void damage(int damage) = 0;
        virtual void addObserver(std::shared_ptr<IObserver> observer) = 0;
        virtual void notifyObservers(std::shared_ptr<INPC> other) = 0;
        virtual void notifyObservers() = 0;
        virtual bool isAlive() = 0;

        virtual void move(const Coordinates& coordinates) = 0;



    protected:
        virtual void setCoordinates(Coordinates coordinates) = 0;
        virtual bool canMove(const Coordinates& coordinates) = 0;
        virtual ~INPC() = default;
};




class NPC : public INPC {
    public:

        NPC(std::string name, std::string type, const int health, const Coordinates coordinates, const NPCDistance npcDistance) :
    name(std::move(name)), type(std::move(type)), health(health), coordinates(coordinates), npcDistance(npcDistance){ };

        std::string getName() const override;
        std::string getType() const override;
        int getHealth() const override;
        Coordinates getCoordinates() const override;
        std::string status() override;
        std::string getInfo() const override;

        double getMoveDistance() const override {return npcDistance.moveDistance;};
        double getAttackDistance() const override {return npcDistance.attackDistance;};

        void damage(const int damage) override {
            health -= damage;
        }

        void addObserver(std::shared_ptr<IObserver> observer) override;
        void notifyObservers(std::shared_ptr<INPC> other) override;
        void notifyObservers() override;
        bool isAlive() override;
        void move(const Coordinates& coordinates) override;



        ~NPC() override = default;

    private:
        void setCoordinates(Coordinates coordinates) override;
        bool canMove(const Coordinates& coordinates) override;
    private:
        std::string name;
        std::string type;
        int health;
        Coordinates coordinates;
        std::vector<std::shared_ptr<IObserver>> observers = {};
        NPCDistance npcDistance;
};

class Dragon final: public NPC {
    public:
        Dragon(std::string name, const int health, const Coordinates coordinates) :
    NPC(std::move(name), "Dragon", health, coordinates, {50, 30}) {};

};

class Knight final: public NPC {
    public:
        Knight(std::string name, const int health, const Coordinates coordinates) :
    NPC(std::move(name), "Knight", health, coordinates, {30, 10}) {};

};

class Princess final: public NPC {
    public:
        Princess(std::string name, const int health, const Coordinates coordinates) :
    NPC(std::move(name), "Princess", health, coordinates, {1, 1}) {};

};