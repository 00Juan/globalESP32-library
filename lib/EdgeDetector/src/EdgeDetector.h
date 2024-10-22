
#ifndef __EDGEDETECTOR
#define __EDGEDETECTOR
#include <Arduino.h>
namespace GlobalLibrary
{
    enum EdgeMode
    {
        EDGE_RISING,
        EDGE_FALLING,
        EDGE_TOGGLE
    };

    enum PinInputMode
    {
        PIN_INPUT,
        PIN_INPUT_PULLUP
    };

    class EdgeDetector
    {
    public:
        // Constructor for pin edge detection with pin mMode and debounce time
        EdgeDetector(uint8_t _pin, EdgeMode _mode, PinInputMode _pinMode, uint32_t _debounceTime)
            : mPin(_pin), mMode(_mode), mDebounceTime(_debounceTime), mNumActivations(0)
        {
            // Set pin mMode based on input
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
            mDigitalReadStatus = mLastState; // Initialize with the initial pin state
        }

        // Update function for edge detection on hardware pin
        void update()
        {
            bool currentState = (digitalRead(mPin) == 0) ? false : true;
            uint32_t currentTime = millis();

            // Check if state has changed and debounce time has passed
            
            if (currentState != mLastState && (currentTime - mLastDebounceTime >= mDebounceTime))
            {
                // Detect the edge based on mMode
                if ((mMode == EDGE_RISING && currentState && !mLastState) ||
                    (mMode == EDGE_FALLING && !currentState && mLastState) ||
                    (mMode == EDGE_TOGGLE))
                {
                    mEdgeDetected = true;
                    mNumActivations++;
                }
                else
                {
                    mEdgeDetected = false;
                }
                mLastDebounceTime = currentTime; // Reset debounce time
                
            }
            else
            {
                mEdgeDetected = false; // Reset if no edge detected
            }
            if(currentTime - mLastDebounceTime >= mDebounceTime)
            {
                mDigitalReadStatus = currentState;
            }

            mLastState = currentState; // Store current state
        }

        // Check if an edge has been detected
        bool getEdgeDetected()
        {
            return mEdgeDetected;
        }

        // Get the debounced pin state after debounce completes
        bool getDigitalRead()
        {
            return mDigitalReadStatus; // Returns the last debounced state
        }

        // Get the number of times the edge was activated
        uint32_t getNumActivations()
        {
            return mNumActivations;
        }

    private:
        uint8_t mPin;               // GPIO pin number (optional)
        EdgeMode mMode;             // Edge detection mode
        uint32_t mDebounceTime;     // Debounce time in milliseconds
        uint32_t mLastDebounceTime; // Last time edge was detected
        uint32_t mNumActivations;   // Number of times the edge has triggered
        bool mLastState;            // Last known state
        bool mEdgeDetected;         // Flag for edge detection
        bool mDigitalReadStatus;    // The debounced digital state of the pin
    };
}


#endif
