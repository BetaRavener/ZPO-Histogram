#include "person.h"
#ifdef PERSON_IVAN
#include <iostream>
#include <string>

#include <opencv2/highgui/highgui.hpp>

#include "grayscale_image.h"
#include "full_histogram.h"
#include "undersampled_histogram.h"
#include "evaluator.h"

int main(int argc, char *argv[])
{
    std::string winName = "Window";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    GrayscaleImage lena("img/lena.png");
    GrayscaleImage lena_under(lena);
    lena_under.scale_values(0.2);

    FullHistogram full_hist;
    UndersampledHistogram under_hist;
    full_hist.compute(lena);
    under_hist.compute(lena, &lena_under);

    Evaluator::compare_histrograms_text(full_hist, under_hist);
    lena_under.show(winName);
    cv::waitKey(0);
	return 0;
}
#endif
