#include "EdgeDetector.h"

namespace GlobalLibrary
{
    // Constructor for pin edge detection with pin mode and debounce time
    EdgeDetector::EdgeDetector(uint8_t _pin, EdgeMode _mode, PinInputMode _pinMode, uint32_t _debounceTime)
        : mPin(_pin), mMode(_mode), mDebounceTime(_debounceTime), mNumActivations(0),
          mCallbackRising(nullptr), mCallbackFalling(nullptr)
    {
        if (_pinMode == PIN_INPUT)
        {
            pinMode(mPin, INPUT);
        }
        else if (_pinMode == PIN_INPUT_PULLUP)
        {
            pinMode(mPin, INPUT_PULLUP);
        }
        mLastState = digitalRead(mPin);
        mLastDebounceTime = millis();
        mEdgeDetected = false;
        mDigitalReadStatus = mLastState;
    }

    // Constructor with callback support
    EdgeDetector::EdgeDetector(uint8_t _pin, EdgeMode _mode, PinInputMode _pinMode, uint32_t _debounceTime, void (*callbackRising)(), void (*callbackFalling)())
        : mPin(_pin), mMode(_mode), mDebounceTime(_debounceTime), mNumActivations(0),
          mCallbackRising(callbackRising), mCallbackFalling(callbackFalling)
    {
        if (_pinMode == PIN_INPUT)
        {
            pinMode(mPin, INPUT);
        }
        else if (_pinMode == PIN_INPUT_PULLUP)
        {
            pinMode(mPin, INPUT_PULLUP);
        }
        mLastState = digitalRead(mPin);
        mLastDebounceTime = millis();
        mEdgeDetected = false;
        mDigitalReadStatus = mLastState;
    }

    void EdgeDetector::update()
    {
        bool currentState = (digitalRead(mPin) == 0) ? false : true;
        uint32_t currentTime = millis();

        // Check if state has changed and debounce time has passed
        if (currentState != mLastState && (currentTime - mLastDebounceTime >= mDebounceTime))
        {
            if ((mMode == EDGE_RISING && currentState && !mLastState) ||
                (mMode == EDGE_FALLING && !currentState && mLastState) ||
                (mMode == EDGE_TOGGLE))
            {
                mEdgeDetected = true;
                mNumActivations++;

                // Execute the appropriate callback
                if (currentState && mMode != EDGE_FALLING)
                {
                    executeCallback(EDGE_RISING);
                }
                else if (!currentState && mMode != EDGE_RISING)
                {
                    executeCallback(EDGE_FALLING);
                }
            }
            else
            {
                mEdgeDetected = false;
            }

            mLastDebounceTime = currentTime;
        }
        else
        {
            mEdgeDetected = false;
        }

        if (currentTime - mLastDebounceTime >= mDebounceTime)
        {
            mDigitalReadStatus = currentState;
        }

        mLastState = currentState;
    }

    bool EdgeDetector::getEdgeDetected()
    {
        return mEdgeDetected;
    }

    bool EdgeDetector::getDigitalRead()
    {
        return mDigitalReadStatus;
    }

    uint32_t EdgeDetector::getNumActivations()
    {
        return mNumActivations;
    }

    void EdgeDetector::setCallbacks(void (*callbackRising)(), void (*callbackFalling)())
    {
        mCallbackRising = callbackRising;
        mCallbackFalling = callbackFalling;
    }

    void EdgeDetector::executeCallback(EdgeMode mode)
    {
        if (mode == EDGE_RISING && mCallbackRising != nullptr)
        {
            mCallbackRising();
        }
        else if (mode == EDGE_FALLING && mCallbackFalling != nullptr)
        {
            mCallbackFalling();
        }
    }
}
