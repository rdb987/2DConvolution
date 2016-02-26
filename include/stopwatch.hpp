#ifndef CGLIFE_STOPWATCH_HPP
#define CGLIFE_STOPWATCH_HPP

#include <chrono>

class stopwatch {
private:
    typedef std::chrono::high_resolution_clock clock;
    bool running;
    clock::time_point start_time;
    clock::duration elapsed;
public:
    stopwatch() {
        running = false;
    }
    // Starts the stopwatch.
    void start() {
        if (!running) {
            running = true;
            start_time = clock::now();
        }
    }
    // Stops the stopwatch.
    void  stop() {
        if (running) {
            running = false;
            elapsed += clock::now() - start_time;
        }
    }
    // Resets the elapsed time to 0.
    void reset() {
        elapsed = clock::duration();
    }
    // Returns the total elapsed time in milliseconds.
    // If the stopwatch is running, the elapsed time
    // includes the time elapsed in the current interval.
    long long elapsed_ms() const {
        clock::duration total;
        if (!running) {
            total = elapsed;
        } else {
            total = elapsed + (clock::now() - start_time);
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(total).count();
    }
};

#endif
