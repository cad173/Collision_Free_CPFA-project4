#include "advesary_controller.h"
#include <unistd.h>

advesary_controller::advesary_controller() :
	RNG(argos::CRandom::CreateRNG("argos")),
	InjectionRate(),
    LoopFunctions(NULL),
    tickCounter(),
    FakeDensity(),
    FakeDecayRate(),
    m_pcLEDs(NULL)
{
}

void advesary_controller::Init(argos::TConfigurationNode &node)
{
    // set settings dont forget to set in XML file
    argos::TConfigurationNode settings = argos::GetNode(node, "advesary_controller");
    argos::GetNodeAttribute(settings, "FakeDensity", FakeDensity);
    argos::GetNodeAttribute(settings, "FakeDecayRate", FakeDecayRate);
    argos::GetNodeAttribute(settings, "InjectionRate", InjectionRate);

    // get robots led actuator <"type of actuator"> 
    m_pcLEDs = GetActuator<CCI_LEDsActuator>("leds");
    m_pcLEDs->SetAllColors(CColor::RED);
    controllerID = GetId();
}

void advesary_controller::ControlStep()
{
    tickCounter++;
    if(tickCounter >= InjectionRate)
    {
        InjectFalsePheromone();
        tickCounter = 0;
    }
}

void advesary_controller::Reset()
{
    tickCounter = 0;
}

void advesary_controller::SetLoopFunctions(CPFA_loop_functions* lf)
{
    LoopFunctions = lf;
}


void advesary_controller::InjectFalsePheromone()
{
    // pick random position restricted to left half of arena (negative X)
    argos::CVector2 pheromonePostion;
    argos::CRange<argos::Real> xBounds(LoopFunctions->getForageRangeX().GetMin(), 0.0);
    argos::CRange<argos::Real> yBounds = LoopFunctions->getForageRangeY();
    pheromonePostion.Set(RNG->Uniform(xBounds), RNG->Uniform(yBounds));

    // set fake trial
    argos::CVector2 nestPosition = LoopFunctions->getNestPosition();
    vector <CVector2> fakeTrial = {pheromonePostion, nestPosition};

    // create phermone trail at that position
    argos::Real currentTime = LoopFunctions->getSimTimeInSeconds();
    Pheromone fakePheromone(pheromonePostion,fakeTrial ,currentTime, FakeDecayRate, FakeDensity);
    LoopFunctions->getPheromoneList().push_back(fakePheromone);
}

REGISTER_CONTROLLER(advesary_controller, "advesary_controller")