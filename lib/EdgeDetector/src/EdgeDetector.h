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
        // Constructor for pin edge detection with pin mode and debounce time
        EdgeDetector(uint8_t _pin, EdgeMode _mode, PinInputMode _pinMode, uint32_t _debounceTime);

        // Constructor with callback support for specific edge modes
        EdgeDetector(uint8_t _pin, EdgeMode _mode, PinInputMode _pinMode, uint32_t _debounceTime, void (*callbackRising)(), void (*callbackFalling)());

        // Update function for edge detection on hardware pin
        void update();

        // Check if an edge has been detected
        bool getEdgeDetected();

        // Get the debounced pin state after debounce completes
        bool getDigitalRead();

        // Get the number of times the edge was activated
        uint32_t getNumActivations();

        // Set callback functions for edge detection
        void setCallbacks(void (*callbackRising)(), void (*callbackFalling)());

    private:
        // Execute the appropriate callback based on the edge type
        void executeCallback(EdgeMode mode);

        uint8_t mPin;               // GPIO pin number (optional)
        EdgeMode mMode;             // Edge detection mode
        uint32_t mDebounceTime;     // Debounce time in milliseconds
        uint32_t mLastDebounceTime; // Last time edge was detected
        uint32_t mNumActivations;   // Number of times the edge has triggered
        bool mLastState;            // Last known state
        bool mEdgeDetected;         // Flag for edge detection
        bool mDigitalReadStatus;    // The debounced digital state of the pin

        // Callback function pointers
        void (*mCallbackRising)();  // Pointer to the rising edge callback function
        void (*mCallbackFalling)(); // Pointer to the falling edge callback function
    };
}

#endif
