//
//  sudoku.hh
//  sudoku
//
//  Created by Yuki Inoue on 2013/08/12.
//  Copyright (c) 2013年 Yuki Inoue. All rights reserved.
//

#ifndef sudoku_sudoku_hh
#define sudoku_sudoku_hh

#include <vector>
#include "grid2d.hpp"
#include "Tile.hpp"

#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>

template <int BaseN>
class Sudoku {
public:
    static const int GroupN = BaseN * BaseN;

private:
    Grid2D<Tile<GroupN> > field;


    static void append_other_row_cordinates(int row, int col, std::vector<std::pair<int,int> > &group) {
        for (int c = 0; c<GroupN; c++) {
            if (c == col) continue;
            group.push_back(std::make_pair(row,c));
        }
    }

    static void append_other_col_cordinates(int row, int col, std::vector<std::pair<int, int> > &group) {
        for (int r = 0; r<GroupN; ++r) {
            if (r == row) continue;
            group.push_back(std::make_pair(r, col));
        }
    }

    static void append_other_local_cordinates(int row, int col, std::vector<std::pair<int, int> > &group) {
        const int rowbase = row/BaseN * BaseN;
        const int colbase = col/BaseN * BaseN;
        for (int r=rowbase; r< rowbase + BaseN; ++r)
            for (int c=colbase; c< colbase + BaseN; ++c) {
                if (r==row && c == col) continue;
                group.push_back(std::make_pair(r,c));
            }
    }

public:

    Sudoku() : field(GroupN, GroupN) {}


    int get(int row, int col) const {
        return field.get(row,col).value();
    }

    void reset() {
        for (Tile<GroupN> &tile : field)
            tile.reset();
    }


    void append_candidates(int row, int col, std::vector<int> &v) const {
        field.get(row,col).append_candidates(v);
    }


    bool fix(int row, int col, int value) {
        if (!field.get(row,col).fixable(value))
            return false;
        if (field.get(row,col).fixed())
            return true;
        field.get(row,col).fix(value);

        std::vector<std::pair<int, int> > groups[3];
        append_other_row_cordinates(row, col, groups[0]);
        append_other_col_cordinates(row, col, groups[1]);
        append_other_local_cordinates(row, col, groups[2]);

        for (const auto &group : groups)
            for (const auto &cord : group)
                if (!reduce(cord.first, cord.second, value))
                    return false;

        return true;
    }

    bool reduce(int row, int col, int value){
        if (!field.get(row,col).reducible(value))
            return false;
        if (!field.get(row,col).fixable(value))
            return true;

        field.get(row,col).reduce(value);

        std::vector<std::pair<int,int> > groups[3];
        append_other_row_cordinates(row, col, groups[0]);
        append_other_col_cordinates(row, col, groups[1]);
        append_other_local_cordinates(row, col, groups[2]);

        for (const auto &group : groups) {
            std::unique_ptr<std::pair<int,int> > fix_candidate;
            bool fix_flag = true;
            for (const auto &cord : group)
                if (field.get(cord.first, cord.second).fixable(value)) {
                    if (fix_candidate) {
                        fix_flag = false;
                        break;
                    }
                    else
                        fix_candidate.reset(new std::pair<int,int>(cord));
                }

            bool result = true;
            if (fix_candidate) {
                if (fix_flag)
                    result = fix(fix_candidate->first, fix_candidate->second, value);
            }
            else {
                result = false;
            }

            if (!result)
                return false;
        }

        return true;
    }
};


template <int BaseN>
bool solve(Sudoku<BaseN> &sudoku) {
    for (int r=0; r<Sudoku<BaseN>::GroupN; ++r)
        for (int c=0; c<Sudoku<BaseN>::GroupN; ++c) {
            std::vector<int> v;
            sudoku.append_candidates(r,c,v);
            if (v.size() == 1)
                continue;
            for (int candidate : v) {
                Sudoku<BaseN> candidate_sudoku = sudoku;
                if(candidate_sudoku.fix(r,c,candidate) && solve(candidate_sudoku)){
                    sudoku = candidate_sudoku;
                    return true;
                }
            }
            return false;
        }
    return true;
}


template <int BaseN>
std::ostream &operator<<(std::ostream &os, const Sudoku<BaseN> &sudoku) {
    static const int groupN = Sudoku<BaseN>::GroupN;
    static const int field_width
    = std::max(2, static_cast<int>(ceil(log10(groupN))));

    for (int r=0; r<groupN; ++r) {
        for (int c=0; c<groupN; ++c)
            std::cout << std::setw(field_width) << sudoku.get(r,c);
        std::cout << std::endl;
    }

    return os;
}


template <int BaseN>
std::istream &operator>>(std::istream &is, Sudoku<BaseN> &sudoku) {
    sudoku.reset();
    const int groupN = Sudoku<BaseN>::GroupN;

    for (int r=0; r<groupN; ++r) {
        for (int c=0; c<groupN; ++c) {
            int v;
            is >> v;
            if (v >= 0)
                sudoku.fix(r,c,v);
        }
    }

    return is;
}



typedef Sudoku<3> Sudoku99;


#endif
