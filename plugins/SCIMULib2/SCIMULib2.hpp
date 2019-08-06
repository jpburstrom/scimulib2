// PluginSCIMULib2.hpp
// Johannes Burström (johannes@ljud.org)

#pragma once

#include "SC_PlugIn.hpp"
#include "RTIMULib.h"
#include <thread>

namespace SCIMULib2 {

//Singleton
class RTIMU_Singleton { 
    public: 
        RTIMU_Singleton();
        ~RTIMU_Singleton();
        static RTIMU_Singleton* get();
        static RTIMU_Singleton* instance;
        void setOrientation(float& roll, float& pitch, float& yaw);
    private:
        bool running;
        RTIMU* imu;
        std::thread* thread;
        void thready();
        float r, p, y;
};

class SCIMULib2 : public SCUnit
{
public:
    SCIMULib2();

    // Destructor
    // ~SCIMULib2();

private:
    // Calc function
    void next(int nSamples);
    float roll;
    float pitch;
    float yaw;

    // Member variables
};

} // namespace SCIMULib2
