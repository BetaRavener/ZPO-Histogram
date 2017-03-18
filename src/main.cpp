#include <iostream>

#include "grayscale_image.h"
#include "full_histogram.h"
#include "undersampled_histogram.h"
#include "evaluator.h"

int main(int argc, char *argv[])
{
    GrayscaleImage lena("img/lena.png");
    FullHistogram full_hist;
    UndersampledHistogram under_hist;
    full_hist.compute(lena);
    under_hist.compute(lena);
    Evaluator::compare_histrograms(full_hist, under_hist);
	return 0;
}
