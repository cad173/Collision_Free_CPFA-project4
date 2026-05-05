#include "ACPFA_controller.h"
#include <unistd.h>

ACPFA_controller::ACPFA_controller()  {}

void ACPFA_controller::Init(argos::TConfigurationNode &node)
{
    CPFA_controller::Init(node);

    m_pcLEDs = GetActuator<CCI_LEDsActuator>("leds");
    m_pcLEDs->SetAllColors(CColor::RED);

    fakePheromoneTimer = 300; // set time between fake pheromone placements (in control steps)
    fakePheromoneStrength = 1.0; // set strength of fake pheromones
    fakePheromoneDecay = 25; // set decay time of fake pheromones 
}


void ACPFA_controller::ControlStep()
{
    CPFA_controller::ControlStep();

    if (fakePheromoneCounter >= fakePheromoneTimer)
    {
        FakePheromoneTrail();
        fakePheromoneCounter = 0;
    }
    else
    {
        fakePheromoneCounter++;
    }
}

void ACPFA_controller::FakePheromoneTrail()
{
    if(LoopFunctions == nullptr) return;

    CVector2 fakeTarget = GetPosition();

    // Build a fake trail (reuse your own movement history)
    std::vector<CVector2> fakeTrail = TrailToShare;

    if(fakeTrail.empty()) {
        fakeTrail.push_back(fakeTarget);
    }

    Real timeInSeconds = (Real)(SimulationTick() / SimulationTicksPerSecond());

    // Fake resource density (controls attractiveness)
    size_t fakeDensity = 50;

    Pheromone fakePheromone(
        fakeTarget,
        fakeTrail,
        timeInSeconds,
        LoopFunctions->RateOfPheromoneDecay,
        fakeDensity,
        GetId(), // use robot ID to identify fake pheromones
        true // isFake = true
    );

    LoopFunctions->PheromoneList.push_back(fakePheromone);
}
// Adversary should not follow pheromone trails, so we override this function to do nothing
bool ACPFA_controller::SetTargetPheromone() {return false;}


// Fidelity should not be used by the adversary as we do not want it to return to the same location, so we override these functions to do nothing
void ACPFA_controller::Returning()
{
    // Force behavior: ignore fidelity completely
    if (IsInTheNest())
    {
        SetRandomSearchLocation();

        isInformed = false;
        isUsingSiteFidelity = false;
        updateFidelity = false;

        CPFA_state = DEPARTING;
        isHoldingFood = false;

        return;
    }

    // Otherwise keep normal movement toward nest
    /*
    if (IsAtTarget())
    {
        SetTarget(LoopFunctions->NestPosition);
    }
    */
}


// No need for this as adversary should not use site fidelity, so we override this function to do nothing
void ACPFA_controller::SetFidelityList()
{
    SiteFidelityPosition = CVector2(10000, 10000);
    updateFidelity = false;
}

REGISTER_CONTROLLER(ACPFA_controller, "ACPFA_controller")