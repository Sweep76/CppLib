/*******************************************************************************
 * Timer.h
 *
 * Author: zhangyu
 * Date: 2017.5.9
 ******************************************************************************/

#pragma once
#include <chrono>

/**
 * Timer, used to measure the running time of a program.
 * Provides a static method to generate a millisecond precision timestamp.
 */
class Timer
{
private:
    size_t time;
public:
    Timer() { time = time_millis(); }
    
    // Generate a millisecond precision timestamp
    static size_t time_millis();
    // Start timing
    void start() { time = time_millis(); } 
    // Reset timing
    void reset() { time = time_millis(); } 
    // Check the total seconds from the start of timing to the current time
    double elapsed() { return (time_millis() - time) / 1000.0; }
};

/**
 * Generate a millisecond precision timestamp.
 *
 * @return A millisecond precision timestamp
 */
size_t Timer::time_millis()
{
    using millis = std::chrono::milliseconds;
    using system_clock = std::chrono::system_clock;
    return std::chrono::duration_cast<millis>(system_clock::now().time_since_epoch()).count();
}
