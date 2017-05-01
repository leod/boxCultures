#include "input_stream.hh"

#include <iostream>

class TestReader : public InputReader<std::string> {
public:
    TestReader(InputStream<std::string> &stream)
        : InputReader<std::string>(stream) {
    }

    void processInput(const std::string &input) {
        std::cout << input << std::endl;
    }
};

int main(int argc, const char **argv) {
    InputStream<std::string> stream;

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

    stream.write("c");
    stream.dispatch();

    return 0;
}
