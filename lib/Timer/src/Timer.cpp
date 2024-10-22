#include "Timer.h"

namespace GlobalLibrary
{
    //Default constructor
    Timer::Timer()
        : mType(CYCLIC), mDuration(1000), mStartTime(0), mRunning(false), mLastState(false), mReset(false), mpCallbackOn(nullptr), mpCallbackOff(nullptr), mTimerOutput(false) {}

    // Constructor to initialize with a timer type and duration
    Timer::Timer(TimerType type, uint32_t duration)
        : mType(type), mDuration(duration), mStartTime(0), mRunning(false), mLastState(false), mReset(false), mpCallbackOn(nullptr), mpCallbackOff(nullptr), mTimerOutput(false) {}

    // Constructor to initialize with a timer type, duration and callback
    Timer::Timer(TimerType type, uint32_t duration, void (*callbackOn)())
        : mType(type), mDuration(duration), mStartTime(0), mRunning(false), mLastState(false), mReset(false), mpCallbackOn(callbackOn), mpCallbackOff(nullptr), mTimerOutput(false) {}

    // Constructor to initialize PWM timer
    Timer::Timer(uint32_t TOn, uint32_t TOff)
        : mType(PWM), mTOn(TOn), mTOff(TOff), mStartTime(0), mRunning(false), mLastState(false), mReset(false), mpCallbackOn(nullptr), mpCallbackOff(nullptr), mTimerOutput(false) {}

    Timer::Timer(uint32_t TOn, uint32_t TOff, void (*callbackOn)(), void (*callbackOff)())
        : mType(PWM), mTOn(TOn), mTOff(TOff), mStartTime(0), mRunning(false), mLastState(false), mReset(false), mpCallbackOn(callbackOn), mpCallbackOff(callbackOff), mTimerOutput(false) {}

    void Timer::update(bool enable)
    {
        bool edgeDetected = false;

        if (mType == TON)
        {
            edgeDetected = !mLastState && enable; // Rising edge
        }
        else if (mType == TOFF)
        {
            edgeDetected = mLastState && !enable; // Falling edge
        }

        if (edgeDetected || mReset)
        {
            start();
        }
        else if ((mType == TON && !enable) || (mType == TOFF && enable))
        {
            stop();
        }

        if (mType == CYCLIC && enable)
        {
            if (getTimerExpired() || !mRunning)
            {
                executeCallback(0);
                start();
                mTimerOutput = !mTimerOutput;
            }
        }

        if (mType == PWM && enable)
        {
            if (getPwmExpired() || !mRunning)
            {
                mCurrentPwmState = !mCurrentPwmState;
                if (mCurrentPwmState)
                    executeCallback(0);
                else
                    executeCallback(1);
                startPwm();
                mTimerOutput = !mTimerOutput;
            }

            mLastState = enable;
        }

        if (getTimerExpired() && (mType == TON || mType == TOFF))
        {
            executeCallback(0);
            mTimerOutput = true;
        }
    }

    void Timer::reset()
    {
        mReset = true;
    }

    void Timer::setCallback(void (*callbackOn)())
    {
        mpCallbackOn = callbackOn;
    }

    void Timer::setCallback(void (*callbackOn)(), void (*callbackOff)())
    {
        mpCallbackOn = callbackOn;
        mpCallbackOff = callbackOff;
    }

    void Timer::setDuration(uint32_t duration)
    {
        mDuration = duration;
    }

    uint32_t Timer::getElapsedTime()
    {
        return mRunning ? (millis() - mStartTime) : 0;
    }

    bool Timer::getTimerOutput()
    {
        return mTimerOutput;
    }

    void Timer::start()
    {
        mStartTime = millis();
        mRunning = true;
        mReset = false;
    }

    void Timer::startPwm()
    {
        mStartTime = millis();
        mRunning = true;
        mDuration = mCurrentPwmState ? mTOn : mTOff;
    }

    bool Timer::getTimerExpired()
    {
        return mRunning && (millis() - mStartTime >= mDuration);
    }

    bool Timer::getPwmExpired()
    {
        uint32_t currentDuration = mCurrentPwmState ? mTOn : mTOff;
        return mRunning && (millis() - mStartTime >= currentDuration);
    }

    void Timer::stop()
    {
        mRunning = false;
        mTimerOutput = false;
    }

    void Timer::executeCallback(int type)
    {
        if (mpCallbackOn != nullptr)
        {
            if (type == 0)
            {
                mpCallbackOn();
            }
            else if (type == 1 && mpCallbackOff != nullptr)
            {
                mpCallbackOff();
            }
        }
    }

}
