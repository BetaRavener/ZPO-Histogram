#ifndef CHESS_HISTOGRAM_H
#define CHESS_HISTOGRAM_H

#include "histogram_base.h"
#include <iostream> 

class ChessHistogram : public HistogramBase
{
  public:
      ChessHistogram(int step=2);
      virtual ~ChessHistogram() = default;
  
      virtual void compute(const GrayscaleImage& img, GrayscaleImage* mark_img=nullptr);
      virtual std::string to_string(bool with_params=false) const;
  private:
      int _step;
};

#endif