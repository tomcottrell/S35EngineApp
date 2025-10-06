#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

// Add this include at the top of your Model.cpp file:
extern "C" {
    #include "../../../EC_code/Inc/SystemHealthManager.h"
}

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
    // Your existing tick code here...

    // Add system health monitoring
    static uint16_t healthUpdateCounter = 0;
    healthUpdateCounter++;

    if (healthUpdateCounter >= 1000)  // Update every ~1 second
    {
        healthUpdateCounter = 0;
        SystemHealth_UpdateMetrics();

        // Optional: Notify listeners if you want push-based updates
        // modelListener->notifySystemHealthUpdate();
    }
}
