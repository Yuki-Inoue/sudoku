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
    Sudoku99 sudoku;
    // std::cin >> sudoku;

    if(solve(sudoku)) {
        std::cout << "could not solve" << std::endl;
        std:: cout << sudoku << std::endl;
    }
    else
        std::cout << sudoku << std::endl;


    return 0;
}

