#include "gtest/gtest.h"
#include "NPC.hpp"
#include "Factory.hpp"
#include "Visitor.hpp"
#include <vector>
#include <memory>
#include <sstream>


TEST(NPCTest, CreateAndAccess) {
    Coordinates coords = {10.0f, 20.0f};
    Knight knight("Knight1", 100, coords);

    EXPECT_EQ(knight.getType(), "Knight");
    EXPECT_EQ(knight.getName(), "Knight1");
    EXPECT_FLOAT_EQ(knight.getCoordinates().x, 10.0f);
    EXPECT_FLOAT_EQ(knight.getCoordinates().y, 20.0f);
}


TEST(KnightFactoryTest, CreateRandomKnight) {
    KnightFactory factory("../data/test.txt");

    auto knight = factory.creatNpc();
    ASSERT_NE(knight, nullptr);
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_FALSE(knight->getName().empty());
    auto coords = knight->getCoordinates();
    EXPECT_GE(coords.x, 0);
    EXPECT_LE(coords.x, 500);
    EXPECT_GE(coords.y, 0);
    EXPECT_LE(coords.y, 500);
}


TEST(ObserverTest, NotifyObservers) {
    auto console_observer = std::make_shared<ConsoleObserver>();
    Knight knight("Knight1", 100, Coordinates{100.0f, 200.0f});
    knight.addObserver(console_observer);

    // Симуляция вызова уведомления
    testing::internal::CaptureStdout();
    knight.notifyObservers();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(output, "Name: Knight1 Type: Knight Health: 100\n");
}
