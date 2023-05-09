#pragma once
#include <chrono>

// A simple timer class that measures elapsed time in seconds between
class Timer
{
public:
    /// <summary>
    /// Saves the current time as the start time.
    /// </summary>
    void start()
    {
        start_time = std::chrono::high_resolution_clock::now();
    }

    /// <summary>
    /// Saves the current time as the stop time.
    /// </summary>
    void stop()
    {
        stop_time = std::chrono::high_resolution_clock::now();
    }

    /// <summary>
    /// Measures the elapsed time between the start and stop times.
    /// </summary>
    /// <returns>The elapsed time in seconds as a double.</returns>
    double measure()
    {
        // Calculate the duration between the start and stop times and convert it to microseconds.
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
        // Convert the duration to seconds as a double and return it.
        return duration.count() / 1000000.0;
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;   // The time when the start method was called.
    std::chrono::high_resolution_clock::time_point stop_time;    // The time when the stop method was called.
};