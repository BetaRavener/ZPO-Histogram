#ifndef CONCENTRIC_HISTOGRAM_H
#define CONCENTRIC_HISTOGRAM_H

#include "histogram_base.h"
#include <iostream> 

class ConcentricHistogram : public HistogramBase
{
  public:
      ConcentricHistogram(int step=2);
      virtual ~ConcentricHistogram() = default;
  
      virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
      virtual std::string to_string(bool with_params=false) const;
  private:
      int _step;
};

#endif