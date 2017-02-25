#include <iostream>

#include "Client.hh"

int main(int argc, char *argv[]) {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet" << std::endl;
        return 1;
    }
    
    Client client("dummy");
    client.connect("localhost", 1234);
    
	while (true) {
        std::vector<Message> messages = client.update();

        for (const auto &message : messages) {
            switch (message.type) {
                case Message::SERVER_CONNECT:
                    std::cout << "Got connected!!!!!!!!11" << std::endl;
                    break;
                case Message::SERVER_TICK: {
                    std::cout << "Start tick" << std::endl;
                    Message message(Message::CLIENT_TICK_DONE);
                    client.sendMessage(message);
                    break;
                }
                case Message::SERVER_START:
                    std::cout << "Game start" << std::endl;
                    break;
                default:
                    assert(false);
            }
        }
	}

    enet_deinitialize();

    return 0;
}
