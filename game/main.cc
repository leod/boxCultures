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

    std::ofstream file("test.json");
    InputRecorder<cereal::JSONOutputArchive> input_logger(file, stream);

    TestReader reader1(stream);

    stream.write("hello");
    stream.write("world");

    stream.dispatch();

    {
        TestReader reader2(stream);

        stream.write("a");
        stream.write("b");

        stream.dispatch();
    }

    input_logger.finish_frame();

    stream.write("c");
    stream.dispatch();

    input_logger.finish_frame();

    return 0;
}
