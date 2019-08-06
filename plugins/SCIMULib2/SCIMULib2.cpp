// PluginSCIMULib2.cpp
// Johannes Burström (johannes@ljud.org)

#include "SC_PlugIn.hpp"
#include "SCIMULib2.hpp"

static InterfaceTable *ft;

namespace SCIMULib2 {

RTIMU_Singleton::RTIMU_Singleton() {
  //debug("MPU9250_Singleton ctor")
    RTIMUSettings* settings = new RTIMUSettings("RTIMULib");
    imu = RTIMU::createIMU(settings);
    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {
        //TODO: error handling
        running = false;
        Print("No IMU found\n");
    } else {
        imu->IMUInit();
        imu->setSlerpPower(0.02);
        imu->setGyroEnable(true);
        imu->setAccelEnable(true);
        imu->setCompassEnable(true);
        running = true;
        thread = new std::thread(&RTIMU_Singleton::thready, this);
    }
}

RTIMU_Singleton::~RTIMU_Singleton() {
    r = p = y = 0.0;
    running = false;
    thread->join();
};

RTIMU_Singleton* RTIMU_Singleton::get() {
    if (RTIMU_Singleton::instance == nullptr) {
        RTIMU_Singleton::instance = new RTIMU_Singleton();
    }
    return RTIMU_Singleton::instance;
}

void RTIMU_Singleton::setOrientation(float& roll, float& pitch, float& yaw){
    roll = r;
    pitch = p;
    yaw = y;
};
void RTIMU_Singleton::thready() {
    int i = 0;
    RTIMU_DATA data;
    while (running) {
        usleep(imu->IMUGetPollInterval() * 1000);
        while (imu->IMURead()) {
            data = imu->getIMUData();
            r = data.fusionPose.x();
            p = data.fusionPose.y();
            y = data.fusionPose.z();
        }
        std::this_thread::yield();
    }
}

RTIMU_Singleton* RTIMU_Singleton::instance = nullptr;

SCIMULib2::SCIMULib2()
{
    mNumOutputs = 3;
    RTIMU_Singleton::get();
    set_calc_function<SCIMULib2, &SCIMULib2::next>();
    next(1);
}

void SCIMULib2::next(int nSamples)
{
    const float * input = in(0);

    float *r = out(0);
    float* p = out(1);
    float* y = out(2);

    RTIMU_Singleton::get()->setOrientation(roll, pitch, yaw);

    for (int i = 0; i < nSamples ; i++) {
        r[i] = roll;
        p[i] = pitch; 
        y[i] = yaw; 
    }

}

} // namespace SCIMULib2

PluginLoad(SCIMULib2UGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<SCIMULib2::SCIMULib2>(ft, "SCIMULib2");
}
