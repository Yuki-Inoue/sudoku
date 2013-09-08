//
//  main.cpp
//  sudoku
//
//  Created by Yuki Inoue on 2013/08/12.
//  Copyright (c) 2013年 Yuki Inoue. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include "sudoku.hpp"


int main(int argc, const char * argv[])
{
    Sudoku99 sudoku;
    std::cin >> sudoku;

    solve(sudoku);
    std::cout << sudoku;


    return 0;
}

