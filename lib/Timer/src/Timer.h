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
        // Constructor to initialize with a timer type and duration
        Timer(TimerType type, uint32_t duration)
            : mType(type), mDuration(duration), mStartTime(0), mRunning(false), mLastState(false), mReset(false) {}

        // Constructor to initialize with a timer type, duration and callback
        Timer(TimerType type, uint32_t duration, void (*callbackOn)())
            : mType(type), mDuration(duration), mStartTime(0), mRunning(false), mLastState(false), mReset(false), mpCallbackOn(callbackOn) {}

        // Constructor to initialize PWM timer
        Timer(uint32_t TOn, uint32_t TOff)
            : mType(PWM), mTOn(TOn), mTOff(TOff), mStartTime(0), mRunning(false), mLastState(false), mReset(false) {}

        Timer(uint32_t TOn, uint32_t TOff,void (*callbackOn)(),void (*callbackOff)())
            : mType(PWM), mTOn(TOn), mTOff(TOff), mStartTime(0), mRunning(false), mLastState(false), mReset(false),mpCallbackOn(callbackOn),mpCallbackOff(callbackOff) {}

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
                    executeCallback(0); // Call the custom function on expiry
                    start();           // Automatically restart the timer when it expires
                    mTimerOutput=!mTimerOutput;
                }
            }

            // Handle PWM timer behavior
            if (mType == PWM && enable)
            {
                if (getPwmExpired() || !mRunning)
                {
                    // Toggle between tON and tOFF
                    mCurrentPwmState = !mCurrentPwmState;
                    if(mCurrentPwmState)  executeCallback(0);  
                    else  executeCallback(1);  
                    startPwm(); // Restart with appropriate duration
                    mTimerOutput=!mTimerOutput;
                }

                mLastState = enable;
            }

            // Execute the callback function if the timer has expired for TON/TOFF
            if (getTimerExpired() && (mType == TON || mType == TOFF))
            {
                executeCallback(0); // Call the custom function on expiry
                mTimerOutput=true;
            }
        }

        // Timer reset
        void reset()
        {
            mReset = true;
        }

        // Set the callback function to be executed when the timer expires
        void setCallback(void (*callbackOn)())
        {
            mpCallbackOn = callbackOn;
        }

        // Set the callback functions for PWM
        void setCallback(void (*callbackOn)(),void (*callbackOff)() )
        {
            mpCallbackOn = callbackOn;
            mpCallbackOff = callbackOff;
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

        bool getTimerOutput()
        {
            return mTimerOutput;
        }

    private:
        // Start the timer
        void start()
        {
            mStartTime = millis();
            mRunning = true;
            mReset = false; // Reset the reset flag
        }

        // Start the PWM timer with the correct duration
        void startPwm()
        {
            mStartTime = millis();
            mRunning = true;
            mDuration = mCurrentPwmState ? mTOn : mTOff;
        }

        // Check if the timer has expired
        bool getTimerExpired()
        {
            return mRunning && (millis() - mStartTime >= mDuration);
        }

        // Check if the PWM timer has expired
        bool getPwmExpired()
        {
            uint32_t currentDuration = mCurrentPwmState ? mTOn : mTOff;
            return mRunning && (millis() - mStartTime >= currentDuration);
        }

        // Stop the timer
        void stop()
        {
            mRunning = false;
            mTimerOutput=false;
        }

        // Execute the user-defined callback function when the timer expires (type=0 is callbackOn, 1 callbackOff)
        void executeCallback(int type)
        {
            if (mpCallbackOn != nullptr)
            {
                if(type==0)
                {
                    mpCallbackOn(); // Call the function if it is set
                }
                else if(type==1)
                {
                    mpCallbackOff();
                }
                
            }
        }

        TimerType mType;     // Type of timer (TON, TOFF, CYCLIC)
        uint32_t mDuration;  // Duration for timing in milliseconds
        uint32_t mStartTime; // Start time for the timer
        bool mRunning;       // Timer running state
        bool mLastState;     // Last monitored value state
        bool mReset;         // Flag to indicate a manual reset
        bool mTimerOutput;   // It will be set/reset to 1 or 0 depending on the timer type
        void (*mpCallbackOn)(); // Pointer to the callback function
        void (*mpCallbackOff)(); // Pointer to the callback function

        // PWM-specific members
        bool mPwmMode;         // Indicates if the timer is in PWM mode
        uint32_t mTOn;         // PWM on duration
        uint32_t mTOff;        // PWM off duration
        bool mCurrentPwmState; // Current state: true if in tON, false if in tOFF
    };

}
#endif