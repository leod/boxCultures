#include "Order.hh"

#include "BitStream.hh"

#include <cassert>

void read(BitStreamReader &reader, Order &order) {
    read(reader, order.player);
    read(reader, order.type);

    switch (order.type) {
    case Order::UNDEFINED:
        assert(false);
        return;
    case Order::BUILD:
        read(reader, order.build.type);
        read(reader, order.build.x);
        read(reader, order.build.y);
        return;
    }
}

void write(BitStreamWriter &writer, const Order &order) {
    write(writer, order.player);
    write(writer, order.type);

    switch (order.type) {
    case Order::UNDEFINED:
        assert(false);
        return;
    case Order::BUILD:
        write(writer, order.build.type);
        write(writer, order.build.x);
        write(writer, order.build.y);
        return;
    }
}
