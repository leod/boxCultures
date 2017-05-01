#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "input_stream.hh"
#include "input_log.hh"

#include <cereal/archives/json.hpp>

#include <iostream>

class TestReader : public InputReader<std::string> {
public:
    TestReader(InputStream<std::string> &stream)
        : InputReader<std::string>(stream) {
    }

    void process_input(const std::string &input) {
        std::cout << input << std::endl;
    }
};

int main(int argc, const char **argv) {
    InputStream<std::string> stream;

    cereal::JSONOutputArchive archive(std::cout);
    InputLogger<cereal::JSONOutputArchive, std::string> input_logger(archive, stream);

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
