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

public:
    typedef typename std::vector<Value>::iterator iterator;
    typedef typename std::vector<Value>::const_iterator const_iterator;

private:

    int height, width;
    std::vector<Value> content;

    int index(int row, int col) const {
        return row*width + col;
    }

public:
    Grid2D(int height_, int width_, const Value &v = Value())
    : height(height_), width(width_), content(height_ * width_, v)
    {}


    const Value &get(int row, int col) const {
        return content[index(row,col)];
    }

    Value &get(int row, int col) {
        return content[index(row,col)];
    }

    Value &operator[](const std::pair<int,int> &p) {
        return get(p.first, p.second);
    }

    const Value &operator[](const std::pair<int,int> &p) const {
        return get(p.first, p.second);
    }


    iterator begin() {
        return content.begin();
    }

    iterator end() {
        return content.end();
    }

    const_iterator begin() const {
        return content.begin();
    }

    const_iterator end() const {
        return content.end();
    }
};

#endif