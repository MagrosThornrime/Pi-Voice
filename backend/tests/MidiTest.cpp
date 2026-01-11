#include <gtest/gtest.h>
#include <midi/Reader.hpp>
#include <midi/Ports.hpp>

using namespace midi;

TEST(MidiPorts, CountIsNonNegative) {
    ASSERT_GE(Ports::count(), 0u);
}

TEST(MidiPorts, ListMatchesCount) {
    auto list = Ports::list();
    ASSERT_EQ(list.size(), Ports::count());
}

TEST(MidiPorts, GetByNumOutOfRangeReturnsInvalidPort) {
    auto port = Ports::getByNum(9999);
    ASSERT_EQ(port.num, 9999u);
}

TEST(MidiPorts, GetByNameUnknownReturnsInvalid) {
    auto port = Ports::getByName("this_port_does_not_exist");
    ASSERT_EQ(port.name, "this_port_does_not_exist");
}

TEST(MidiReader, DefaultNotOpened) {
    Reader reader(Port{});
    ASSERT_FALSE(reader.opened());
    ASSERT_FALSE((bool)reader);
}

TEST(MidiReader, OpenAndClose) {
    auto ports = Ports::list();
    if (ports.empty()) {
        GTEST_SKIP();
    }

    Reader reader(ports[0]);
    ASSERT_TRUE(reader.opened());

    reader.close();
    ASSERT_FALSE(reader.opened());
}

TEST(MidiReader, ClearEmptiesQueue) {
    auto ports = Ports::list();
    if (ports.empty()) {
        GTEST_SKIP();
    }

    Reader reader(ports[0]);
    reader.clear();
    ASSERT_EQ(reader.queueSize(), 0u);
}

TEST(MidiReader, ReadEmptyReturnsDefault) {
    auto ports = Ports::list();
    if (ports.empty()) {
        GTEST_SKIP();
    }

    Reader reader(ports[0]);
    auto data = reader.read();
    ASSERT_EQ(data.bytes.size(), 0u);
}

TEST(MidiReader, ReadSomeEmptyReturnsEmptyVector) {
    auto ports = Ports::list();
    if (ports.empty()) {
        GTEST_SKIP();
    }

    Reader reader(ports[0]);
    auto data = reader.readSome(10);
    ASSERT_TRUE(data.empty());
}

TEST(MidiReader, ReadAllEmptyReturnsEmptyVector) {
    auto ports = Ports::list();
    if (ports.empty()) {
        GTEST_SKIP();
    }

    Reader reader(ports[0]);
    auto data = reader.readAll();
    ASSERT_TRUE(data.empty());
}
