#include "masked_histogram.h"

using namespace std;

MaskedHistogram::MaskedHistogram(int step) :
    _step(step)
{
}   

void MaskedHistogram::compute(const GrayscaleImage& img, GrayscaleImage* mark_img)
{
    clear_data();
    
    int width = img.width()/3;
    int height = img.height()/3;
    width = round(width);
    height = round(height);
    int i;
    int j;
   
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int coordx2 = 0;
    int coordy2 = 0;
        
    vector<int> mask = {0, 3,2,3, 2,1,2, 3,2,3};
    int index = 1;
    int innerStep = 0;
    
    cout << img.width() << endl;
    cout << img.height() << endl;
    
    if(_step >= 1 )
    {       
        for(int a = 0; a < img.width(); a = a + width)
        {
            if((a + width) > img.width())
            { 
                continue;    
            }
            for(int b = 0; b < img.height(); b = b + height)
            {
                if((b + height) > img.height())
                { 
                    continue;    
                }
                
                if((a +(2*width)) > img.width())
                {
                    coordx2 = img.width()-1;    
                }
                else
                {
                    coordx2 = a + width;    
                }
                
                if((b +(2*height)) > img.height())
                {
                    coordy2 = img.height()-1;    
                }
                else
                {
                    coordy2 = b + height;
                }
                
                x1 = a;
                y1 = b;
                x2 = coordx2;
                y2 = coordy2;
                innerStep = mask[index] + _step;
                index++;
                 
                                                    
                while(x1 <= x2 && y1 <= y2)
                {
                  //---------------------------------
                    for(i = x1; i <= x2; i++)
                    {
                        _data[img.pixel(i, y1)]++;
                        _data[img.pixel(i, y2)]++; 
                        _used_samples = _used_samples + 2;
                        if(mark_img != nullptr)
                        {
                              mark_img->pixel(i, y1, 255);
                              mark_img->pixel(i, y2, 255);       
                        }          
                    }
                    
                    for(j = y1; j <= y2; j++)
                    {
                        _data[img.pixel(x1,j)]++;
                        _data[img.pixel(x2,j)]++; 
                        _used_samples = _used_samples + 2;
                        if(mark_img != nullptr)
                        {
                            mark_img->pixel(x1, j, 255);
                            mark_img->pixel(x2, j, 255);          
                        }       
                    }    
                  //---------------------------------
                    x1 += innerStep;
                    y1 += innerStep;
                    x2 -= innerStep;          
                    y2 -= innerStep;                
                }  
            }
         }
     }   
 
    // Normalize histogram
    for (int i = 0; i < size; i++)
    {
        _data[i] /= _used_samples;
    } 
}

std::string MaskedHistogram::to_string(bool with_params) const
{
    return "Masked Histogram" + (with_params ?
            (" (" + std::to_string(_step) + ")") : "");
}