#include "AdversaryController.h"
#include <argos3/core/utility/math/rng.h>

using namespace argos;

static int FORCE_KEEP = [](){
    std::cerr << "ADV FORCE LINK ACTIVE\n";
    return 0;
}();

CAdversaryController::CAdversaryController() :
    m_unStepCounter(0),
    m_unNextDecisionStep(0)
{
    std::cerr << "CONSTRUCTOR HIT" << std::endl;
    LOG << "[ADV] Controller constructed" << std::endl;
}

void CAdversaryController::Init() {
    LOG << "[ADV] Adversary initialized: " << GetId() << std::endl;
}

void CAdversaryController::Reset() {
    m_unStepCounter = 0;
    m_unNextDecisionStep = 0;
    LOG << "[ADV] Adversary spawned / reset: " << GetId() << std::endl;
    Stop();
}

void CAdversaryController::ControlStep() {

    m_unStepCounter++;

    // If currently idle or reached decision point, pick new action
    if(GetMovementState() == STOP || m_unStepCounter >= m_unNextDecisionStep) {
        ChooseNextAction();
    }

    // execute movement pipeline from BaseController
    Move();
}

void CAdversaryController::ChooseNextAction() {

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

REGISTER_CONTROLLER(Adversary_controller, "adversary")
