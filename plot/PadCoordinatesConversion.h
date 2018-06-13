#ifndef PadCoordinatesConversion_h_
#define PadCoordinatesConversion_h_

// Found at
// https://root.cern.ch/phpBB3/viewtopic.php?t=20666
// and ruthlessly modified
#include "TPad.h"

double GetGPadWidth()
{
  return gPad->GetAbsWNDC() * gPad->GetWw();
}

double GetGPadHeight()
{
  return gPad->GetAbsHNDC() * gPad->GetWh();
}

// Before actual conversions - font related functions which relate to pad
// sizes

bool IsPixelFont(const int font)
{
  return font % 10 == 3;
}

double GetGPadSizeForFonts()
{
  return std::min(GetGPadWidth(), GetGPadHeight());
}

double GetFontSizePixels(const double ndc)
{
  return ndc * GetGPadSizeForFonts();
}

double GetFontSizeNDC(const double p)
{
  return p / GetGPadSizeForFonts();
}

double GetFontSizePixels(const int font, const double size)
{
  return IsPixelFont(font) ? size : GetFontSizePixels(size);
}

double GetFontSizeNDC(const int font, const double size)
{
  return IsPixelFont(font) ? GetFontSizeNDC(size) : size;
}

// in case of log axes, e.g. gPad->GetUxmax() does not return the value of
// TAxis::GetXmax(), but rather log10(TAxis::GetXmax())

double AxisToUser(const double x, const bool logx)
{
  return logx ? std::pow(10.0, x) : x;
}

double AxisToX(const double ax)
{
  return AxisToUser(ax, gPad->GetLogx());
}

double AxisToY(const double ay)
{
  return AxisToUser(ay, gPad->GetLogy());
}

double UserToAxis(const double x, const bool logx)
{
  return logx ? std::log10(x) : x;
}

double XtoAxis(const double x)
{
  return UserToAxis(x, gPad->GetLogx());
}

double YtoAxis(const double y)
{
  return UserToAxis(y, gPad->GetLogy());
}

double GetUxmin()
{
  gPad->Update();
  return gPad->GetUxmin();
}

double GetUxmax()
{
  gPad->Update();
  return gPad->GetUxmax();
}

double GetUymin()
{
  gPad->Update();
  return gPad->GetUymin();
}

double GetUymax()
{
  gPad->Update();
  return gPad->GetUymax();
}

double GetFrameWidthNDC()
{
  return 1.0 - gPad->GetLeftMargin() - gPad->GetRightMargin();
}

double GetFrameHeightNDC()
{
  return 1.0 - gPad->GetTopMargin() - gPad->GetBottomMargin();
}

double GetFrameWidthAxis()
{
  return GetUxmax() - GetUxmin();
}

double GetFrameHeightAxis()
{
  return GetUymax() - GetUymin();
}

double GetRatioWidthNDCAxis()
{
  return GetFrameWidthNDC() / GetFrameWidthAxis();
}

double GetRatioHeightNDCAxis()
{
  return GetFrameHeightNDC() / GetFrameHeightAxis();
}

double NDCtoX(const double ndcx)
{
  const double ax =
    (ndcx - gPad->GetLeftMargin()) / GetRatioWidthNDCAxis() + GetUxmin();
  return AxisToX(ax);
}

double NDCtoY(const double ndcy)
{
  const double ay =
    (ndcy - gPad->GetBottomMargin()) / GetRatioHeightNDCAxis() + GetUymin();
  return AxisToY(ay);
}

double XtoNDC(const double x)
{
  const double ax = XtoAxis(x);
  return GetRatioWidthNDCAxis() * (ax - GetUxmin()) + gPad->GetLeftMargin();
}

double YtoNDC(const double y)
{
  const double ay = YtoAxis(y);
  return GetRatioHeightNDCAxis() * (ay - GetUymin()) +
    gPad->GetBottomMargin();
}

double NDCtoPixelX(const double ndcx)
{
  return ndcx * GetGPadWidth();
}

double NDCtoPixelY(const double ndcy)
{
  return (1.0 - ndcy) * GetGPadHeight();
}

double PixelXtoNDC(const double px)
{
  return px / GetGPadWidth();
}

double PixelYtoNDC(const double py)
{
  return 1.0 - py / GetGPadHeight();
}

double PixelToX(const double px)
{
  return NDCtoX(PixelXtoNDC(px));
}

double PixelToY(const double py)
{
  return NDCtoY(PixelYtoNDC(py));
}

double XtoPixel(const double x)
{
  return NDCtoPixelX(XtoNDC(x));
}

double YtoPixel(const double y)
{
  return NDCtoPixelY(YtoNDC(y));
}


#endif
