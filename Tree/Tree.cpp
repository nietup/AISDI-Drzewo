// Tree.cpp : Defines the entry point for the console application.
//
/**
@file main.cc

Plik z funkcja main() do cwiczenia 4 na laboratoriach z AISDI.

@author
Pawel Cichocki, Michal Nowacki

@date
last revision
- 2005.12.01 Michal Nowacki: lab #4
- 2005.10.27 Pawel Cichocki: added some comments
- 2005.10.26 Michal Nowacki: creation - separated from another file

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include "stdafx.h"
#include <iostream>

#include "TreeMap.h"

int CCount::count = 0;

int Test2 ();
void test ();

int main () {
    std::cout << "AISDI tree: wchodze do funkcji main." << std::endl;
    test ();
    // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
    //Test2 ();
    //system("PAUSE");

    std::cout << "\nDUPA" << std::endl;
    std::cin.get ();
    return EXIT_SUCCESS;
}
