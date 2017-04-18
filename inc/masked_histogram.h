#ifndef MASKED_HISTOGRAM_H
#define MASKED_HISTOGRAM_H

#include "histogram_base.h"
#include <iostream> 

class MaskedHistogram : public HistogramBase
{
  public:
      MaskedHistogram(int step=2);
      virtual ~MaskedHistogram() = default;
  
      virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
      virtual std::string to_string(bool with_params=false) const;
  private:
      int _step;
};

#endif