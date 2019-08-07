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
        void getOrientation(float& a, float& b, float& c);
        void saveOffset();
    private:
        bool running;
        bool savingOffset;
        RTQuaternion offset;
        RTIMU* imu;
        std::thread* thread;
        void thready();
        float w, x, y, z;
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
    float x, y, z;
    float m_trig;

    // Member variables
};

} // namespace SCIMULib2
