#include <Arduino.h>
#include <FSM.h>
#include <EdgeDetector.h>
#include <Timer.h>
using namespace GlobalLibrary;

// Declare State functions
void onState1Enter() { Serial.println("Entering State 1"); }
void onState1Update() { Serial.println("Updating State 1"); }
void onState1Exit() { Serial.println("Exiting State 1"); }

void onState2Enter() { Serial.println("Entering State 2"); }
void onState2Update() { Serial.println("Updating State 2"); }
void onState2Exit() { Serial.println("Exiting State 2"); }

// Create states
State state1(onState1Enter, NULL, onState1Exit);
State state2(onState2Enter, NULL, onState2Exit);

// Initialize FSM with the first state
FiniteStateMachine fsm(&state1);

EdgeDetector bot1(16, EDGE_FALLING, PIN_INPUT_PULLUP, 100);
EdgeDetector bot2(17, EDGE_FALLING, PIN_INPUT_PULLUP, 100);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    // Update the FSM
    fsm.update();
    bot1.update();
    bot2.update();

    bool stsTrigB1 = bot1.getEdgeDetected();
    bool stsTrigB2 = bot2.getEdgeDetected();

    // Example transition after 5 seconds
    if (stsTrigB1)
    {
        fsm.transitionTo(&state2);
    }
    else if(stsTrigB2)
    {
        fsm.transitionTo(&state1);
    }
}
