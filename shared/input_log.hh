#pragma once

#include "for_each_in_tuple.hh"

#include <vector>

template<typename T>
class BufferedInputReader : public InputReader<T> {
    std::vector<T> buffer;

public:
    BufferedInputReader(InputStream<T> &stream);

    void process_input(const T &input);

    const std::vector<T> &get() const;
    void clear();
};

template<typename Archive>
struct WriteLogFunctor {
    Archive& archive;

    WriteLogFunctor(Archive& archive)
        : archive(archive) {
    }

    template<typename T>
    void operator()(BufferedInputReader<T>& reader) {
        archive(reader.get());
        reader.clear();
    }
};

template<typename Archive, typename... Ts>
class InputLogger {
    Archive& archive;

    std::tuple<BufferedInputReader<Ts>...> readers;

public:
    InputLogger(Archive& archive, InputStream<Ts> &... streams);

    void finish_frame();
};

template<typename T>
class LogInputSource : public InputSource {

};

template<typename T>
BufferedInputReader<T>::BufferedInputReader(InputStream<T> &stream)
    : InputReader<T>(stream) {
}

template<typename T>
void BufferedInputReader<T>::process_input(const T &input) {
    buffer.push_back(input);
}

template<typename T>
const std::vector<T> &BufferedInputReader<T>::get() const {
    return buffer;
}

template<typename T>
void BufferedInputReader<T>::clear() {
    buffer.clear();
}

template<typename Archive, typename... Ts>
InputLogger<Archive, Ts...>::InputLogger(Archive& archive, InputStream<Ts> &... streams) :
    archive(archive), readers(streams...) {
}

template<typename Archive, typename... Ts>
void InputLogger<Archive, Ts...>::finish_frame() {
    for_each_in_tuple(readers, WriteLogFunctor<Archive>(archive));
}
