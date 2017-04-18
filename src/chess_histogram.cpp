#include "chess_histogram.h"

using namespace std;

ChessHistogram::ChessHistogram(int step) :
    _step(step)
{
}   


void ChessHistogram::compute(const GrayscaleImage& img, GrayscaleImage* mark_img)
{
    clear_data();
    int i;
    int j;
    int k;
    int l;
    bool swap = false;
    int tmp;
    int x2 = img.width() -1;
    int y2 = img.height() -1;
    
    if(_step >= 1)
    {    
        while(1)
        {    
            for(i = 0; i < x2; i = i + _step)                
            {   
                tmp = round(y2/_step);                    
               
                if(tmp % 2 == 0)
                {
                    swap = !swap;       
                }
                swap = !swap;    
                for(j = 0; j < y2; j = j + _step)
                {                                       
                    swap = !swap;                                                                                 
                    if(swap)
                    {    
                        for(k = i; k < (i + _step); k++)
                        {                                
                            for(l = j; l < (j + _step); l++)
                            {   
                                if(k <= x2 && l <= y2)
                                {    
                                    _data[img.pixel(k,l)]++; 
                                    _used_samples++;        
                                    if(mark_img != nullptr)
                                    {
                                        mark_img->pixel(k, l, 255);                                            
                                    } 
                                }
                            }                             
                        }
                        
                    }                                          
                }                
            }
            break;
        }
    }
    // Normalize histogram
    for (int i = 0; i < size; i++)
    {
        _data[i] /= _used_samples;
    } 
}

std::string ChessHistogram::to_string(bool with_params) const
{
    return "Chess Histogram" + (with_params ?
            (" (" + std::to_string(_step) + ")") : "");
}