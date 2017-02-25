#ifndef STRAT_COMMON_ORDER_HH
#define STRAT_COMMON_ORDER_HH

#include "Defs.hh"

struct BitStreamReader;
struct BitStreamWriter;

struct Order {
    enum Type : uint8_t {
        UNDEFINED,

        BUILD
    } type;

    Order(Type type = UNDEFINED)
        : type(type) {
    }

    PlayerId player;

    union {
        struct {
            BuildingType type;
            uint16_t x, y;
        } build;
    };
};

void read(BitStreamReader &, Order &);
void write(BitStreamWriter &, const Order &);

#endif
