/* 
 * File:   main.cpp
 * Author: Shashwat Shivam
 *
 */

#include "Interface.h"
#include<random>
#include<iostream>

int main()
{
    // srand(5);
    auto t=time(0);
    // auto t=1537180015;
    // cout<<"# RANDOM SEED- "<<t<<endl;
    srand(t);

    Inteface *interface = new Inteface();

    return 0;
}
