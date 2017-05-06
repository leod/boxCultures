// Implementation of input logging by registering as input readers to streams.
// The advantage of recording input is the possibility of reproducing bugs by
// simply replaying the input. For this purpose, any external input that
// influences our program (e.g. time passed, network messages, user input)
// needs to be logged, and the response to the input needs to be deterministic.
// Input recording can be used in a similar fashion to create replays of
// games, by recording the orders given by the players in each tick of the
// simulation.
// Here, we use cereal to serialize inputs of arbitrary type into an archive.

#pragma once

#include "input_stream.hh"

#include <cereal/cereal.hpp>

#include <vector>
#include <ostream>
#include <memory>

// In the log, streams are identified by an id. The stream id 0 is reserved 
// to signify the end of a frame.
const size_t INPUT_RECORDING_FRAME_FINISHED = 0;

template<typename Archive>
class InputRecorder {
private:
    // Serialization output stream
    std::ostream& ostream;
    Archive archive;

    // Each reader logs the input of one stream to the output archive. We use
    // an empty base class to erase the input type.
    class ReaderBase {
    public:
        virtual ~ReaderBase() {
        }
    };

    std::vector<std::unique_ptr<ReaderBase>> readers;


    // Writes the input from one stream in a batch when that stream's
    // ``dispatch'' method is called. Afterwards, the output stream is flushed
    // (to still record the input in case the game crashes).
    template<typename T>
    class Reader : public ReaderBase, InputReader<T> {
    public:
        InputRecorder& recorder;

        // This id identifies the stream in the output log
        size_t stream_id;
        
        Reader(InputStream<T>& stream, InputRecorder& recorder,
               size_t stream_id)
            : InputReader<T>(stream),
              recorder(recorder), stream_id(stream_id) {
        }

        void process_input(std::vector<T> const& input) {
            recorder.archive(CEREAL_NVP(stream_id));
            recorder.archive(input);
            recorder.ostream.flush();
        }
    };

    template<typename T>
    void create_readers(InputStream<T>& stream) {
        size_t stream_id = INPUT_RECORDING_FRAME_FINISHED + 1 +
                           readers.size();
        readers.emplace_back(new Reader<T>(stream, *this, stream_id)); 
    }

    template<typename T, typename... Ts>
    void create_readers(InputStream<T>& stream, Ts&... tail) {
        create_readers(stream);
        create_readers(tail...);
    }

public:
    // Construct an input recorder to serialize to an archive in ``ostream''.
    // An arbitrary number of streams can be logged into one file. Note that
    // the order of the given streams is important, as it is used to create
    // ids for the streams. When the log is to be read later, the order of
    // the streams needs to be the same or there will be undefined behavior.
    template<typename... Ts>
    InputRecorder(std::ostream& ostream, Ts&... streams)
        : ostream(ostream), archive(ostream) {
        create_readers(streams...); 
    }

    // Finish logging of the current frame by writing the reserved stream id.
    // When the log is read later, this makes it possible to replay input
    // frame by frame.
    void finish_frame() {
        archive(cereal::make_nvp("stream_id", INPUT_RECORDING_FRAME_FINISHED));
    }
};

