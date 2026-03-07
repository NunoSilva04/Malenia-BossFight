/**********************************************************************************
*   File: Rect.h
* 
*   Description: 
*        Template struct for a rectangle.
* 
*   See: 
*       docs/Math_Lib/Rect.md 
*       for detailed documentation.
* 
*************************************************************************************/

#ifndef __RECT_H__
#define __RECT_H__

#include <iostream>

namespace n_math{
    template<typename T>
    struct rect{
        static_assert(std::is_same<T, double>::value || std::is_same<T, float>::value || std::is_same<T, int>::value
            , "Can only be of the type int, float, double.");
        T x, y, width, heigth;

        rect(): x(0), y(0), width(20), heigth(10) { }
        rect(T x, T y, T w, T h){
            this->x = x;
            this->y = y;
            this->width = w;
            this->heigth = h;
        }
        void print(void){
            std::cout << "x = " << x << ", y = " << y << ", width = " << width << ", heigth= " << heigth << "\n";            
            return;
        }
    };
};

#endif