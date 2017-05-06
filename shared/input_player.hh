#pragma once

#include "input_stream.hh"

#include <istream>

// Replay input by reading it from a cereal archive and writing into input 
// streams. This class closely mirrors the ``InputRecorder''.
template<typename Archive>
class InputPlayer : InputSource {
private:
    // Serialization input
    Archive archive; 

    // Each writer reads from the recording archive and writes to its
    // associated stream
    class WriterBase {
    public:
        virtual ~WriterBase() {
        }

        virtual void update() = 0;
    };

    std::vector<std::unique_ptr<WriterBase>> writers;

    // Implementation of the input writer for one specific input type
    template<typename T>
    class Writer : public WriterBase {
    public:
        // Associated stream to write to
        InputStream<T>& stream;

        InputPlayer& input_player;

        // Member variable in order to reuse memory between frames
        std::vector<T> inputs;

        Writer(InputStream<T>& stream, InputPlayer& input_player)
            : stream(stream), input_player(input_player) {
        }

        void update() {
            input_player.archive(inputs);
            for (T const& input : inputs)
                stream.write(input);

            inputs.clear();
        }
    };

    template<typename T>
    void create_writers(InputStream<T>& stream) {
        writers.emplace_back(new Writer<T>(stream, *this));
    }

    template<typename T, typename... Ts>
    void create_writers(InputStream<T>& stream, Ts&... tail) {
        create_writers(stream);
        create_writers(tail...);
    }

public:
    // Construct an input source that reads from a recording created by an
    // ``InputRecorder''. Note that the order of the streams given here needs
    // to be the same as given to the input recorder.
    template<typename... Ts>
    InputPlayer(std::istream& istream, Ts&... streams)
        : archive(istream) {
        create_writers(streams...);
    }

    // Write input from one frame into the streams
    void update();
};

template<typename Archive>
void InputPlayer<Archive>::update() {
    // Id of the stream to which the next input batch goes
    size_t stream_id;
    archive(stream_id);

    while (stream_id != INPUT_RECORDING_FRAME_FINISHED) {
        // Validate stream id
        size_t stream_index = stream_id - (INPUT_RECORDING_FRAME_FINISHED + 1);

        if (stream_index >= writers.size())
            throw std::runtime_error("Invalid stream id in input recording");

        // Read input list
        writers[stream_index]->update();

        // Read next stream id
        archive(stream_id);
    }
}
