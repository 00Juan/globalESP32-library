#ifndef __TIMER
#define __TIMER
#include <Arduino.h>

namespace GlobalLibrary
{

    enum TimerType
    {
        TON,
        TOFF,
        CYCLIC
    };

    class Timer
    {
    public:
        // Constructor to initialize with a timer type and duration
        Timer(TimerType type, uint32_t duration)
            : mType(type), mDuration(duration), mStartTime(0), mRunning(false), mLastState(false), mReset(false) {}

        // Update the timer based on the monitored boolean value (enable)
        void update(bool enable)
        {
            bool edgeDetected = false;

            // Handle TON and TOFF timers
            if (mType == TON)
            {
                edgeDetected = !mLastState && enable; // Rising edge
            }
            else if (mType == TOFF)
            {
                edgeDetected = mLastState && !enable; // Falling edge
            }

            // Start or reset the timer on edge detection or manual reset
            if (edgeDetected || mReset)
            {
                start();
            }
            // Stop the timer if the condition is unmet
            else if ((mType == TON && !enable) || (mType == TOFF && enable))
            {
                stop();
            }

            // Handle CYCLIC timer behavior
            if (mType == CYCLIC && enable)
            {
                if (getTimerExpired() || !mRunning)
                {
                    executeCallback(); // Call the custom function on expiry
                    start();           // Automatically restart the timer when it expires
                }
            }

            mLastState = enable; // Update the last known state

            // Execute the callback function if the timer has expired for TON/TOFF
            if (getTimerExpired() && (mType == TON || mType == TOFF))
            {
                executeCallback(); // Call the custom function on expiry
            }
        }

        // Timer reset
        void reset()
        {
            mReset = true;
        }

        // Check if the timer has expired
        bool getTimerExpired()
        {
            return mRunning && (millis() - mStartTime >= mDuration);
        }

        // Set a new duration for the timer
        void setDuration(uint32_t duration)
        {
            mDuration = duration;
        }

        // Get the elapsed time since the timer started
        uint32_t getElapsedTime()
        {
            return mRunning ? (millis() - mStartTime) : 0;
        }

        // Set the callback function to be executed when the timer expires
        void setCallback(void (*callback)())
        {
            mCallback = callback;
        }

    private:
        // Start the timer
        void start()
        {
            mStartTime = millis();
            mRunning = true;
            mReset = false; // Reset the reset flag
        }

        // Stop the timer
        void stop()
        {
            mRunning = false;
        }

        // Execute the user-defined callback function when the timer expires
        void executeCallback()
        {
            if (mCallback != nullptr)
            {
                mCallback(); // Call the function if it is set
            }
        }

        TimerType mType;     // Type of timer (TON, TOFF, CYCLIC)
        uint32_t mDuration;  // Duration for timing in milliseconds
        uint32_t mStartTime; // Start time for the timer
        bool mRunning;       // Timer running state
        bool mLastState;     // Last monitored value state
        bool mReset;         // Flag to indicate a manual reset
        void (*mCallback)(); // Pointer to the callback function
    };

    class SoftwarePWM
    {
    public:
        // Constructor: initializes the pulse durations and configures the timer
        SoftwarePWM(uint32_t pulseHighDuration, uint32_t pulseLowDuration)
            : mPulseHighDuration(pulseHighDuration), mPulseLowDuration(pulseLowDuration),
              mTimer(TON, pulseHighDuration), mPulseState(false)
        {
            mTimer.setDuration(pulseHighDuration); // Set initial timer duration
        }

        // Update the state of the PWM signal based on the timer
        void update(bool enable)
        {
            mTimer.update(enable);
            // If timer has expired, toggle the PWM state
            if (mTimer.getTimerExpired())
            {
                mPulseState = !mPulseState; // Toggle the PWM state

                // Set the timer duration based on the next state
                uint32_t nextDuration = mPulseState ? mPulseHighDuration : mPulseLowDuration;
                mTimer.setDuration(nextDuration); // Update the timer duration for the new state
                mTimer.reset();
            }
        }

        // Get the current state of the PWM signal (HIGH or LOW)
        bool getState() const
        {
            return mPulseState;
        }

    private:
        uint32_t mPulseHighDuration; // Duration for HIGH state
        uint32_t mPulseLowDuration;  // Duration for LOW state
        Timer mTimer;                // Timer object for managing pulse timing
        bool mPulseState;            // Current state of the pulse (HIGH or LOW)
    };

}
#endif