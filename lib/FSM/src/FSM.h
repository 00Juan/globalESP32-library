#ifndef __FSM
#define __FSM

#include "Arduino.h"

namespace GlobalLibrary
{

  typedef void (*StateCallback)();
  typedef bool (*TransitionGuard)();

  class State
  {
  public:
    State(const char *name, StateCallback onEnter, StateCallback onUpdate, StateCallback onExit, void *data = nullptr);
    void enter();
    void update();
    void exit();
    const char *getName();
    void *getData();
    void setData(void *data);

  private:
    const char *mName;
    StateCallback mOnEnterFunc;
    StateCallback mOnUpdateFunc;
    StateCallback mOnExitFunc;
    void *mpData;
  };

  struct StateMachineData
  {
    State *mpCurrentState;
    unsigned long mLastStateChange;
  };

  class FiniteStateMachine
  {
  public:
    FiniteStateMachine(State *initialState);
    void update();
    bool transitionTo(State *nextState, TransitionGuard guard = nullptr);
    State *getCurrentState();
    const char *getCurrentStateName();
    unsigned long getTimeInCurrentState();

  private:
    StateMachineData mFsmData;
  };
}
#endif
