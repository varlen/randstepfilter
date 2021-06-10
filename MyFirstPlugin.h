#pragma once

#include "IPlug_include_in_plug_hdr.h"

const int kNumPresets = 1;

enum EParams
{
  kSpeed = 0,
  kRessonance = 1,
  kCentralFrequency = 2,
  kFrequencyStep = 3,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class MyFirstPlugin final : public Plugin
{
public:
  MyFirstPlugin(const InstanceInfo& info);

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
#endif
};
