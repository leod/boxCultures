#pragma once

#include "Message.hh"

#include <enet/enet.h>
#include <cassert>

#include <string>

// The client connects to the specified game server,
// and then runs a game simulation with the settings given by the server.
//
// Everytime the server sends a tick to the client, it is
// executed in the local simulation.
struct Client {
    Client(const std::string &username);
    ~Client();

    void connect(const std::string &host, int port);
	std::vector<Message> update();

    void sendOrder(const Order &order);
    void sendMessage(const Message &);

    PlayerId getPlayerId() const {
        assert(playerId > 0);
        return playerId;
    }

private:
    std::string username;

    ENetHost *client;
    ENetPeer *peer;

    GameSettings settings;

    PlayerId playerId;

    void handleMessage(const Message &);
};