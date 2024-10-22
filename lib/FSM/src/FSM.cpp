#include "FSM.h"

// State Constructor
State::State(StateCallback onEnter, StateCallback onUpdate, StateCallback onExit) {
  this->onEnterFunc = onEnter;
  this->onUpdateFunc = onUpdate;
  this->onExitFunc = onExit;
}

// State Methods
void State::enter() {
  if (onEnterFunc) {
    onEnterFunc();
  }
}

void State::update() {
  if (onUpdateFunc) {
    onUpdateFunc();
  }
}

void State::exit() {
  if (onExitFunc) {
    onExitFunc();
  }
}

// FSM Constructor
FiniteStateMachine::FiniteStateMachine(State* initialState) {
  currentState = initialState;
  currentState->enter();
}

// FSM Methods
void FiniteStateMachine::update() {
  if (currentState) {
    currentState->update();
  }
}

void FiniteStateMachine::transitionTo(State* nextState) {
  if (currentState) {
    currentState->exit();
  }
  currentState = nextState;
  if (currentState) {
    currentState->enter();
  }
}
//write a function that will be called when the state is entered
//create a struct to save all the FSM data  
