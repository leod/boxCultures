#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "input_stream.hh"
#include "input_recorder.hh"
#include "input_player.hh"

#include <cereal/archives/json.hpp>

#include <iostream>
#include <fstream>

class TestReader : public InputReader<std::string> {
public:
    TestReader(InputStream<std::string>& stream)
        : InputReader<std::string>(stream) {
    }

    void process_input(const std::string& input) {
        std::cout << input << std::endl;
    }
};

int main(int argc, char const** argv) {
    InputStream<std::string> stream;

    TestReader reader1(stream);

    {
        std::cerr << "Recording" << std::endl;

        std::ofstream file("test.json");
        InputRecorder<cereal::JSONOutputArchive> recorder(file, stream);

        stream.write("hello");
        stream.write("world");

        stream.dispatch();

        {
            TestReader reader2(stream);

            stream.write("a");
            stream.write("b");

            stream.dispatch();
        }

        recorder.finish_frame();

        stream.write("c");
        stream.dispatch();

        recorder.finish_frame();
    }

    {
        std::cerr << "Playing" << std::endl;

        std::ifstream file("test.json");
        InputPlayer<cereal::JSONInputArchive> input_player(file, stream);

        input_player.update();
        stream.dispatch();

        std::cerr << "Frame end" << std::endl;

        input_player.update();
        stream.dispatch();

        std::cerr << "Frame end" << std::endl;
    }

    return 0;
}
