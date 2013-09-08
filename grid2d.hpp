//
//  grid2d.hh
//  sudoku
//
//  Created by Yuki Inoue on 2013/08/12.
//  Copyright (c) 2013年 Yuki Inoue. All rights reserved.
//

#ifndef sudoku_grid2d_hh
#define sudoku_grid2d_hh

#include <utility>
#include <vector>


template <class Value>
class Grid2D {

    int height, width;
    std::vector<Value> content;

    int index(int row, int col) const {
        return row*width + col;
    }

public:
    Grid2D(int height_, int width_)
    : height(height_), width(width_), content(height_ * width_)
    {}


    template <class Ref>
    void set(int row, int col, Ref &&ref){
        content[index(row,col)] = std::forward<Ref>(ref);
    }


    const Value &get(int row, int col) const {
        return content[index(row,col)];
    }

    Value &get(int row, int col) {
        return content[index(row,col)];
    }
};

#endif