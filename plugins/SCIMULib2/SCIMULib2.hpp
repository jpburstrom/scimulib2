// PluginSCIMULib2.hpp
// Johannes Burström (johannes@ljud.org)

#pragma once

#include "SC_PlugIn.hpp"

namespace SCIMULib2 {

class SCIMULib2 : public SCUnit
{
public:
    SCIMULib2();

    // Destructor
    // ~SCIMULib2();

private:
    // Calc function
    void next(int nSamples);

    // Member variables
};

} // namespace SCIMULib2
