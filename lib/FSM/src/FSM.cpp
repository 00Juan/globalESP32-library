#include "Fsm.h"

namespace GlobalLibrary
{
    // State Constructor
    State::State(const char *name, StateCallback onEnter, StateCallback onUpdate, StateCallback onExit, void *data)
        : mName(name), mOnEnterFunc(onEnter), mOnUpdateFunc(onUpdate), mOnExitFunc(onExit), mpData(data) {}

    // State Methods
    void State::enter()
    {
        if (mOnEnterFunc)
        {
            mOnEnterFunc();
        }
    }

    void State::update()
    {
        if (mOnUpdateFunc)
        {
            mOnUpdateFunc();
        }
    }

    void State::exit()
    {
        if (mOnExitFunc)
        {
            mOnExitFunc();
        }
    }

    const char *State::getName()
    {
        return mName;
    }

    void *State::getData()
    {
        return mpData;
    }

    void State::setData(void *data)
    {
        mpData = data;
    }

    // FSM Constructor
    FiniteStateMachine::FiniteStateMachine(State *initialState)
    {
        mFsmData.mpCurrentState = initialState;
        mFsmData.mLastStateChange = millis();
        if (mFsmData.mpCurrentState)
        {
            mFsmData.mpCurrentState->enter();
        }
    }

    // FSM Methods
    void FiniteStateMachine::update()
    {
        if (mFsmData.mpCurrentState)
        {
            mFsmData.mpCurrentState->update();
        }
    }

    bool FiniteStateMachine::transitionTo(State *nextState, TransitionGuard guard)
    {
        if (mFsmData.mpCurrentState == nextState)
        {
            return false; // Avoid transition to the same state
        }

        if (guard && !guard())
        {
            return false; // Guard condition failed
        }

        if (mFsmData.mpCurrentState)
        {
            mFsmData.mpCurrentState->exit();
        }

        mFsmData.mpCurrentState = nextState;
        mFsmData.mLastStateChange = millis();

        if (mFsmData.mpCurrentState)
        {
            mFsmData.mpCurrentState->enter();
        }
        return true;
    }

    State *FiniteStateMachine::getCurrentState()
    {
        return mFsmData.mpCurrentState;
    }

    const char *FiniteStateMachine::getCurrentStateName()
    {
        return mFsmData.mpCurrentState ? mFsmData.mpCurrentState->getName() : "No State";
    }

    unsigned long FiniteStateMachine::getTimeInCurrentState()
    {
        return millis() - mFsmData.mLastStateChange;
    }

}