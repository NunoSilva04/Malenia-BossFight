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
    template<typename T, bool test>
    struct rect{
        T x, y, w, h;
        bool test_value;

        rect(): test_value(test), y(20), x(5), h(3), w(1000) { }
        ~rect(){ }
        void print(void){
            std::cout << "x = " << x << ", y = " << y << ", w = " << w << ", h = " << h << "\n";
            std::cout << "Value choosen: " << test_value << "\n";
            return;
        }
    };
};

#endif