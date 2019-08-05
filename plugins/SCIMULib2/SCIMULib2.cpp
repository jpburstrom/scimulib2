// PluginSCIMULib2.cpp
// Johannes Burström (johannes@ljud.org)

#include "SC_PlugIn.hpp"
#include "SCIMULib2.hpp"

static InterfaceTable *ft;

namespace SCIMULib2 {

SCIMULib2::SCIMULib2()
{
    set_calc_function<SCIMULib2, &SCIMULib2::next>();
    next(1);
}

void SCIMULib2::next(int nSamples)
{
    const float * input = in(0);
    const float * gain = in(0);
    float * outbuf = out(0);

    // simple gain function
    for (int i = 0; i < nSamples; ++i) {
        outbuf[i] = input[i] * gain[i];
    }
}

} // namespace SCIMULib2

PluginLoad(SCIMULib2UGens) {
    // Plugin magic
    ft = inTable;
    registerUnit<SCIMULib2::SCIMULib2>(ft, "SCIMULib2");
}
