#include "Client.hh"
#include "BitStream.hh"

#include <stdexcept>
#include <iostream>
#include <cassert>

Client::Client(const std::string &username)
    : username(username),
      client(NULL),
      peer(NULL),
      playerId(0) {
}

Client::~Client() {
    if (client)
        enet_host_destroy(client);
}

void Client::connect(const std::string &host, int port) {
    std::cout << "Connecting to " << host << ":" << port << std::endl;

    client = enet_host_create(NULL, 1, 2, 0, 0);

    if (client == NULL)
        throw std::runtime_error("Failed to create ENet client");

    ENetAddress address;
    enet_address_set_host(&address, host.c_str());
    address.port = port;

    peer = enet_host_connect(client, &address, 2, 0);

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected!" << std::endl;
    } else {
        enet_peer_reset(peer);

        throw std::runtime_error("Failed to connect");
    }

    // Say hello
    Message message(Message::CLIENT_CONNECT);
    message.client_connect.name = username;
    sendMessage(message);
}

std::vector<Message> Client::update() {
	std::vector<Message> messages;

    ENetEvent event;
    while (enet_host_service(client, &event, 0) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            break;
        case ENET_EVENT_TYPE_RECEIVE: {
            BitStreamReader reader(event.packet->data, event.packet->dataLength);

            Message::Type messageType;
            read(reader, messageType);

            messages.emplace_back(messageType);
            read(reader, messages.back());

            handleMessage(messages.back());

            enet_packet_destroy(event.packet);
            break;
        }
        case ENET_EVENT_TYPE_DISCONNECT:
            std::cout << "Got disconnected" << std::endl;
            break;
        default:
			assert(false);
        }
    }

	return messages;
}

void Client::sendOrder(const Order &order) {
    Order o(order);
    o.player = playerId;

	Message message(Message::CLIENT_ORDER);
	message.client_order.order = o;
	sendMessage(message);
}

void Client::sendMessage(const Message &message) {
    ENetPacket *packet = message.toPacket();
    enet_peer_send(peer, 0, packet);
}

void Client::handleMessage(const Message &message) {
    switch (message.type) {
    case Message::SERVER_CONNECT:
        std::cout << "Connected to server with player id "
                  << message.server_connect.yourPlayerId << std::endl;
        playerId = message.server_connect.yourPlayerId;
        return;

    default:
        return;
    }
}

