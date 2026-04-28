#include "Adversary_controller.h"

using namespace argos;

struct DebugLoad {
    DebugLoad() {
        std::cout << "ADVERSARY CPP IS RUNNING\n";
    }
} debugLoad;

AdversaryController::AdversaryController() :
    BaseController() {
        std::cout << GetPosition() << std::endl;
    }

void AdversaryController::Init(TConfigurationNode &node) {
    compassSensor   = GetSensor<CCI_PositioningSensor>("positioning");
    proximitySensor = GetSensor<CCI_FootBotProximitySensor>("footbot_proximity");
    wheelActuator   = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");

    // Set starting position
    CVector2 p(GetPosition());
    SetStartPosition(CVector3(p.GetX(), p.GetY(), 0.0));

    // Start with a random target so it actually moves
    Real angle = RNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(), CRadians::PI.GetValue()));
    CVector2 randomTarget = p + CVector2(1.0, CRadians(angle));

    SetIsHeadingToNest(false);
    SetTarget(randomTarget);
}

void AdversaryController::ControlStep() {
    // pick new random one 
    if(IsAtTarget()) {
        CVector2 p = GetPosition();

        Real angle = RNG->Uniform(CRange<Real>(-CRadians::PI.GetValue(), CRadians::PI.GetValue()));
        CVector2 newTarget = p + CVector2(1.5, CRadians(angle));

        SetIsHeadingToNest(false);
        SetTarget(newTarget);
    }

    Move();
}

REGISTER_CONTROLLER(AdversaryController, "adversary");

// #include "AdversaryController.h"
// #include <argos3/core/utility/math/rng.h>

// using namespace argos;
// virtual void Init() override;
// virtual void Reset() override;

// static int FORCE_KEEP = [](){
//     std::cerr << "ADV FORCE LINK ACTIVE\n";
//     return 0;
// }();

// AdversaryController::AdversaryController() :
//     m_unStepCounter(0),
//     m_unNextDecisionStep(0)
// {
//     std::cerr << "CONSTRUCTOR HIT" << std::endl;
//     std::cerr << "[ADV] Controller constructed" << std::endl;
// }

// void AdversaryController::Init() {
//     std::cerr << "[ADV] Adversary initialized: " << GetId() << std::endl;
// }

// void AdversaryController::Reset() {
//     m_unStepCounter = 0;
//     m_unNextDecisionStep = 0;
//     std::cerr << "[ADV] Adversary spawned / reset: " << GetId() << std::endl;
//     Stop();
// }

// void AdversaryController::ControlStep() {

//     m_unStepCounter++;

//     // If currently idle or reached decision point, pick new action
//     if(GetMovementState() == STOP || m_unStepCounter >= m_unNextDecisionStep) {
//         ChooseNextAction();
//     }

//     // execute movement pipeline from BaseController
//     Move();
// }

// void AdversaryController::ChooseNextAction() {

//     // reset decision timer (random interval)
//     m_unNextDecisionStep = m_unStepCounter + RNG->Uniform(CRange<UInt32>(10, 40));

//     // random decision
//     Real r = RNG->Uniform(CRange<Real>(0.0, 1.0));

//     if(r < 0.6f) {
//         // mostly forward motion
//         SetMoveForward(RNG->Uniform(CRange<Real>(0.05, 0.25)));
//     }
//     else if(r < 0.8f) {
//         // turn left
//         SetLeftTurn(RNG->Uniform(CRange<Real>(10.0, 90.0)));
//     }
//     else {
//         // turn right
//         SetRightTurn(RNG->Uniform(CRange<Real>(10.0, 90.0)));
//     }
// }

// REGISTER_CONTROLLER(AdversaryController, "adversary")
