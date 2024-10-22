#ifndef __FSM
#define __FSM

#include "Arduino.h"

typedef void (*StateCallback)();

class State {
  public:
    State(StateCallback onEnter, StateCallback onUpdate, StateCallback onExit);
    void enter();
    void update();
    void exit();

  private:
    StateCallback onEnterFunc;
    StateCallback onUpdateFunc;
    StateCallback onExitFunc;
};

class FiniteStateMachine {
  public:
    FiniteStateMachine(State* initialState);
    void update();
    void transitionTo(State* nextState);

  private:
    State* currentState;
};

#endif
