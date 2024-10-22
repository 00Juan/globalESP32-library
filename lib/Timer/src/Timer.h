#ifndef __TIMER
#define __TIMER
#include <Arduino.h>

namespace GlobalLibrary
{

    enum TimerType
    {
        TON,
        TOFF,
        CYCLIC,
        PWM
    };

    class Timer
    {
    public:
        // Default constructor
        Timer();

        // Constructor to initialize with a timer type and duration
        Timer(TimerType type, uint32_t duration);

        // Constructor to initialize with a timer type, duration and callback
        Timer(TimerType type, uint32_t duration, void (*callbackOn)());

        // Constructor to initialize PWM timer
        Timer(uint32_t TOn, uint32_t TOff);

        // Constructor to initialize PWM timer with callbacks
        Timer(uint32_t TOn, uint32_t TOff, void (*callbackOn)(), void (*callbackOff)());

        // Update the timer based on the monitored boolean value (enable)
        void update(bool enable);

        // Timer reset
        void reset();

        // Set the callback function to be executed when the timer expires
        void setCallback(void (*callbackOn)());

        // Set the callback functions for PWM
        void setCallback(void (*callbackOn)(), void (*callbackOff)());

        // Set a new duration for the timer
        void setDuration(uint32_t duration);

        // Get the elapsed time since the timer started
        uint32_t getElapsedTime();

        // Get the current output of the timer
        bool getTimerOutput();

    private:
        // Start the timer
        void start();

        // Start the PWM timer with the correct duration
        void startPwm();

        // Check if the timer has expired
        bool getTimerExpired();

        // Check if the PWM timer has expired
        bool getPwmExpired();

        // Stop the timer
        void stop();

        // Execute the user-defined callback function when the timer expires (type=0 is callbackOn, 1 callbackOff)
        void executeCallback(int type);

        // Attributes
        TimerType mType;         // Type of timer (TON, TOFF, CYCLIC)
        uint32_t mDuration;      // Duration for timing in milliseconds
        uint32_t mStartTime;     // Start time for the timer
        bool mRunning;           // Timer running state
        bool mLastState;         // Last monitored value state
        bool mReset;             // Flag to indicate a manual reset
        bool mTimerOutput;       // It will be set/reset to 1 or 0 depending on the timer type
        void (*mpCallbackOn)();  // Pointer to the callback function
        void (*mpCallbackOff)(); // Pointer to the callback function

        // PWM-specific members
        uint32_t mTOn;         // PWM on duration
        uint32_t mTOff;        // PWM off duration
        bool mCurrentPwmState; // Current state: true if in tON, false if in tOFF
    };

}

#endif
