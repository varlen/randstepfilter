#include "MyFirstPlugin.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"
#include "IPlugPaths.h"

MyFirstPlugin::MyFirstPlugin(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kSpeed)->InitDouble("Rate", 50, 0., 100.0, 1, "%");
  GetParam(kCentralFrequency)->InitFrequency("Cutoff Frequency", 100, 0., 10000, 1);
  GetParam(kFrequencyStep)->InitFrequency("Bandwidth", 200, 1, 2000, 1);
  GetParam(kRessonance)->InitGain("Ressonance", 10, 1, 20, .5);

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    const ISVG knobSVG = pGraphics->LoadSVG(SVGKNOBROTATE_FN);
    const ISVG secondaryKnobSVG = pGraphics->LoadSVG(SVGSECONDARYKNOBROTATE_FN);

    const IColor MY_BACKGROUND_COLOR(255, 17, 87, 173);

    const IVStyle style{
      true, // Show label
      true, // Show value
      {
        MY_BACKGROUND_COLOR, // Background
        DEFAULT_FGCOLOR, // Foreground
        DEFAULT_PRCOLOR, // Pressed
        COLOR_BLACK, // Frame
        DEFAULT_HLCOLOR, // Highlight
        DEFAULT_SHCOLOR, // Shadow
        COLOR_BLACK, // Extra 1
        DEFAULT_X2COLOR, // Extra 2
        DEFAULT_X3COLOR  // Extra 3
      }, // Colors
      IText(20.f, EAlign::Center, COLOR_WHITE) // Label text
    };

    //pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(MY_BACKGROUND_COLOR);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    pGraphics->AttachControl(new ITextControl(b.GetFromTop(50), "Randstepfilter", IText(50, EAlign::Center, COLOR_WHITE)));
    pGraphics->AttachControl(new ITextControl(b.GetFromBRHC(120,20).GetPadded(2), "github.com/varlen", IText(15, EAlign::Center, COLOR_WHITE)));

    // Grid system definitions
    const int nRows = 1;
    const int nCols = 4;
    const float padding = -5.;

    int cellIdx = -1;

    auto nextCell = [&]() {
      return b.GetGridCell(++cellIdx, nRows, nCols).GetPadded(padding);
    };

    auto sameCell = [&]() {
      return b.GetGridCell(cellIdx, nRows, nCols).GetPadded(padding);
    };

    auto AddLabel = [&](const char* label) {
      pGraphics->AttachControl(new ITextControl(nextCell().GetFromTop(PLUG_HEIGHT/2), label, style.labelText));
    };

    AddLabel("Speed");
    pGraphics->AttachControl(new ISVGKnobControl(sameCell().GetCentredInside(100), secondaryKnobSVG, kSpeed), kNoTag, "speed_controls");

    AddLabel("Ressonance");
    pGraphics->AttachControl(new ISVGKnobControl(sameCell().GetCentredInside(100), secondaryKnobSVG, kRessonance), kNoTag, "filter_controls");

    AddLabel("Cutoff");
    pGraphics->AttachControl(new ISVGKnobControl(sameCell().GetCentredInside(100), secondaryKnobSVG, kCentralFrequency), kNoTag, "filter_controls");

    AddLabel("Bandwidth");
    pGraphics->AttachControl(new ISVGKnobControl(sameCell().GetCentredInside(100), secondaryKnobSVG, kFrequencyStep), kNoTag, "filter_controls");

  };
#endif
}

#if IPLUG_DSP
void MyFirstPlugin::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const double gain = GetParam(kSpeed)->Value() / 100.;
  const int nChans = NOutChansConnected();
  
  for (int s = 0; s < nFrames; s++) {
    for (int c = 0; c < nChans; c++) {
      outputs[c][s] = inputs[c][s] * gain;
    }
  }
}
#endif
