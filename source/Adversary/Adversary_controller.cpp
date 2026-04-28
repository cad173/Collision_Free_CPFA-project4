#include "AdversaryController.h"
#include <argos3/core/utility/math/rng.h>

using namespace argos;
virtual void Init() override;
virtual void Reset() override;

static int FORCE_KEEP = [](){
    std::cerr << "ADV FORCE LINK ACTIVE\n";
    return 0;
}();

AdversaryController::AdversaryController() :
    m_unStepCounter(0),
    m_unNextDecisionStep(0)
{
    std::cerr << "CONSTRUCTOR HIT" << std::endl;
    std::cerr << "[ADV] Controller constructed" << std::endl;
}

void AdversaryController::Init() {
    std::cerr << "[ADV] Adversary initialized: " << GetId() << std::endl;
}

void AdversaryController::Reset() {
    m_unStepCounter = 0;
    m_unNextDecisionStep = 0;
    std::cerr << "[ADV] Adversary spawned / reset: " << GetId() << std::endl;
    Stop();
}

void AdversaryController::ControlStep() {

    m_unStepCounter++;

    // If currently idle or reached decision point, pick new action
    if(GetMovementState() == STOP || m_unStepCounter >= m_unNextDecisionStep) {
        ChooseNextAction();
    }

    // execute movement pipeline from BaseController
    Move();
}

void AdversaryController::ChooseNextAction() {

    // reset decision timer (random interval)
    m_unNextDecisionStep = m_unStepCounter + RNG->Uniform(CRange<UInt32>(10, 40));

    // random decision
    Real r = RNG->Uniform(CRange<Real>(0.0, 1.0));

    if(r < 0.6f) {
        // mostly forward motion
        SetMoveForward(RNG->Uniform(CRange<Real>(0.05, 0.25)));
    }
    else if(r < 0.8f) {
        // turn left
        SetLeftTurn(RNG->Uniform(CRange<Real>(10.0, 90.0)));
    }
    else {
        // turn right
        SetRightTurn(RNG->Uniform(CRange<Real>(10.0, 90.0)));
    }
}

REGISTER_CONTROLLER(AdversaryController, "adversary")
