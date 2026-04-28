#ifndef ADVERSARY_CONTROLLER_H
#define ADVERSARY_CONTROLLER_H

#include "BaseController.h"

class CAdversaryController : public BaseController {

public:

    CAdversaryController();

    virtual ~CAdversaryController() {}

    virtual void ControlStep() override;

    virtual void Reset() override;

private:

    argos::UInt32 m_unStepCounter;
    argos::UInt32 m_unNextDecisionStep;

    void ChooseNextAction();

};

#endif