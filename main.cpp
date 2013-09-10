//
//  main.cpp
//  sudoku
//
//  Created by Yuki Inoue on 2013/08/12.
//  Copyright (c) 2013年 Yuki Inoue. All rights reserved.
//

#include <iostream>
#include "sudoku.hpp"


int main(int argc, const char * argv[])
{
    Sudoku<unsigned long> sudoku(3);
    // std::cin >> sudoku;

    if(!sudoku.solve())
        std::cout << "could not solve" << std::endl;
    else
        std::cout << sudoku << std::endl;


    return 0;
}

