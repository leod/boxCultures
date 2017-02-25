#include <iostream>

#include <enet/enet.h>

ENetHost * server = NULL;

int
main () 
{
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet" << std::endl;    
        return 1;
    }

    // Wait for this number of players before starting the game
    size_t numWaitPlayers = 1;

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 1234;

    server = enet_host_create(&address, 32, 2, 0, 0);

    if (server == NULL) {
        std::cerr << "Failed to create host." << std::endl;
        return 1;
    }

    std::cout << "Server started" << std::endl;

    bool quit = false;
    while (!quit) {
        ENetEvent event;

        while (enet_host_service(server, &event, 0) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT: {
                std::cout << "A new client connected" << std::endl;

                ClientInfo *client = new ClientInfo(++playerCounter, event.peer);
                client->player.color = playerCounter % 4;
                client->player.team = playerCounter;

                event.peer->data = client;

                clients.push_back(client);

                break;
            }
            case ENET_EVENT_TYPE_RECEIVE: {
                ClientInfo *client = static_cast<ClientInfo *>(event.peer->data);

                BitStreamReader reader(event.packet->data, event.packet->dataLength);

                Message::Type messageType;
                read(reader, messageType);
                Message message(messageType);
                read(reader, message);

                handleMessage(client, message);

                enet_packet_destroy(event.packet);
                break;
            }
            case ENET_EVENT_TYPE_DISCONNECT: {
                ClientInfo *client = static_cast<ClientInfo *>(event.peer->data);
                std::cout << "Player " << client->player.id << " disconnected" << std::endl;

                auto position = std::find(clients.begin(), clients.end(), client);
                assert(position != clients.end());
                clients.erase(position);

                delete client;

                if (clients.empty()) {
                    std::cout << "All players disconnected" << std::endl;
                    quit = true;
                }

                break;
            }

            default: assert(false);
            }
        }
    }

    enet_host_destroy(server);
    enet_deinitialize();

    return 0;
}
