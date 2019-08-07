// PluginSCIMULib2.cpp
// Johannes Burström (johannes@ljud.org)

#include "SC_PlugIn.hpp"
#include "SCIMULib2.hpp"

static InterfaceTable *ft;

namespace SCIMULib2 {

RTIMU_Singleton::RTIMU_Singleton() {
  //debug("MPU9250_Singleton ctor")
    //TODO: move this initialization to thread
    RTIMUSettings* settings = new RTIMUSettings("RTIMULib");
    RTVector3 vec = RTVector3(0, 0, 0);
    imu = RTIMU::createIMU(settings);
    if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL)) {

        //TODO: error handling
        running = false;
        Print("No IMU found\n");
    } else {
        offset.fromEuler(vec);
        savingOffset = true; //tmp
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
    w = x = y = z = 0.0;
    running = false;
    thread->join();
};

RTIMU_Singleton* RTIMU_Singleton::get() {
    if (RTIMU_Singleton::instance == nullptr) {
        RTIMU_Singleton::instance = new RTIMU_Singleton();
    }
    return RTIMU_Singleton::instance;
}

void RTIMU_Singleton::getOrientation(float& a, float& b, float& c){
    a = x;
    b = y;
    c = z;
};

void RTIMU_Singleton::saveOffset() {
    savingOffset = true;
}

void RTIMU_Singleton::thready() {
    int i = 0;
    RTIMU_DATA data;
    RTQuaternion tmp;
    RTVector3 euler;
    while (running) {
        usleep(imu->IMUGetPollInterval() * 1000);
        while (imu->IMURead()) {
            data = imu->getIMUData();
            if (savingOffset) {
                offset = data.fusionQPose.conjugate();
                savingOffset = false;
            };
            tmp = offset * data.fusionQPose;
            tmp.toEuler(euler);
            x = euler.x();
            y = euler.y();
            z = euler.z();
        }
        std::this_thread::yield();
    }
}

RTIMU_Singleton* RTIMU_Singleton::instance = nullptr;

SCIMULib2::SCIMULib2()
{
    RTIMU_Singleton::get();
    m_trig = -1;
    set_calc_function<SCIMULib2, &SCIMULib2::next>();
    next(1);
}

void SCIMULib2::next(int nSamples)
{
    float trig = in0(0);

    float* o0 = out(0);
    float* o1 = out(1);
    float* o2 = out(2);


	if (trig > 0.f && m_trig <= 0.f) {
        RTIMU_Singleton::get()->saveOffset();
    };

    m_trig = trig;

    RTIMU_Singleton::get()->getOrientation(x, y, z);

    for (int i = 0; i < nSamples ; i++) {
        o0[i] = x;
        o1[i] = y;
        o2[i] = z;
    }

}

} // namespace SCIMULib2

PluginLoad(SCIMULib2UGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<SCIMULib2::SCIMULib2>(ft, "SCIMULib2");
}
