#pragma once

#include <vector>
#include <algorithm>

#include <cassert>

template<typename T>
class InputStream;

// A reader class that is called whenever new input is dispatched
// from an associated stream
template<typename T>
class InputReader {
    // The stream this reader is registered to.
    // A reader belongs to exactly one stream through its lifetime.
    // On destruction, the reader automatically unregisters itself.
    InputStream<T> &stream;

public:
    InputReader(InputStream<T> &stream);
    virtual ~InputReader();

    // This function is called when new input is dispatched
    virtual void processInput(const T &input) = 0;
};

// A stream holds a list of input readers and a queue of inputs.
// Upon dispatch, readers are notified of new input.
template<typename T>
class InputStream {
    // Registered readers
    std::vector<InputReader<T> *> readers;

    // Only InputReader can register/unregister
    friend class InputReader<T>;

    // Input queue
    std::vector<T> inputs;

public:
    // Add one input to the end of the queue
    void write(const T &input);

    // Dispatch all inputs in order from old to new
    void dispatch();
};

// An input source needs to be updated periodically to poll for new events
// to write to a stream
class InputSource {
public:
    virtual void update() = 0;
};

template<typename T>
InputReader<T>::InputReader(InputStream<T> &stream)
    : stream(stream) {
    // Register
    stream.readers.push_back(this);
}

template<typename T>
InputReader<T>::~InputReader() {
    // Unregister
    auto it = std::find(stream.readers.begin(), stream.readers.end(), this);
    assert(it != stream.readers.end());
    stream.readers.erase(it);
}

template<typename T>
void InputStream<T>::write(const T &input) {
    inputs.push_back(input);
}

template<typename T>
void InputStream<T>::dispatch() {
    for (const T &input : inputs)
        for (InputReader<T> *reader : readers)
            reader->processInput(input);

    inputs.clear();
}
