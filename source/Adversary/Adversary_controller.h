#ifndef ADVERSARY_CONTROLLER_H
#define ADVERSARY_CONTROLLER_H

#include "../Base/BaseController.h"

class AdversaryController : public BaseController {
public:
    AdversaryController();
    virtual ~AdversaryController() {}

    virtual void Init(argos::TConfigurationNode &node);
    virtual void ControlStep();
};

#endif

// #ifndef ADVERSARY_CONTROLLER_H
// #define ADVERSARY_CONTROLLER_H

// #include "BaseController.h"

// class AdversaryController : public BaseController {

// public:

//     AdversaryController();

//     virtual ~AdversaryController() {}

//     virtual void ControlStep() override;

//     virtual void Reset() override;

// private:

//     argos::UInt32 m_unStepCounter;
//     argos::UInt32 m_unNextDecisionStep;

//     void ChooseNextAction();

// };

// #endif