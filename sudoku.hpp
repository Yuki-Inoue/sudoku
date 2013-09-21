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

#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <exception>
#include <climits>



/*
 what to return on unfixed tile
 when made to convert into int
 */
static const int UnfixedTile = -2;






template <typename Unsigned>
class BitSizeCalculator {
public:
    static const size_t bitsize =
        sizeof(Unsigned) * CHAR_BIT;
};

template <typename Unsigned>
bool tile_fixed(const Unsigned &u) {
    return !(u&u-1);
}


// default sudoku supports baseN up to 5.
// use unsigned long long for up to 8.
// implement multibyte unsigned integer for more.
//
template <typename Unsigned = unsigned long>
class Sudoku {
private:

    int baseN;
    Grid2D<Unsigned> field;

    std::vector<Unsigned> bitvalue_table;







    void append_other_row_cordinates(int row, int col, std::vector<std::pair<int,int> > &group) {
        for (int c = 0; c<getGroupN(); c++) {
            if (c == col) continue;
            group.push_back(std::make_pair(row,c));
        }
    }

    void append_other_col_cordinates(int row, int col, std::vector<std::pair<int, int> > &group) {
        for (int r = 0; r<getGroupN(); ++r) {
            if (r == row) continue;
            group.push_back(std::make_pair(r, col));
        }
    }

    void append_other_local_cordinates(int row, int col, std::vector<std::pair<int, int> > &group) {
        const int rowbase = row/baseN * baseN;
        const int colbase = col/baseN * baseN;
        for (int r=rowbase; r< rowbase + baseN; ++r)
            for (int c=colbase; c< colbase + baseN; ++c) {
                if (r==row && c == col) continue;
                group.push_back(std::make_pair(r,c));
            }
    }

    bool check_reduces_from_fix(int row, int col, Unsigned bitvalue) {
        std::vector<std::pair<int, int> > groups[3];
        append_other_row_cordinates(row, col, groups[0]);
        append_other_col_cordinates(row, col, groups[1]);
        append_other_local_cordinates(row, col, groups[2]);

        for (const auto &group : groups)
            for (const auto &cord : group) {
                Unsigned &tile = field[cord];
                if (!(tile ^ bitvalue))
                    return false;
                if (!(tile & bitvalue))
                    continue;
                tile ^= bitvalue;
                if (tile_fixed(tile) &&
                    !check_reduces_from_fix(cord.first, cord.second, tile))
                    return false;
            }

        return true;
    }

    bool check_fixable_by_reduction(int row, int col, Unsigned bitvalue) {

        std::vector<std::pair<int,int> > groups[3];
        append_other_row_cordinates(row, col, groups[0]);
        append_other_col_cordinates(row, col, groups[1]);
        append_other_local_cordinates(row, col, groups[2]);

        for (const auto &group : groups) {
            std::unique_ptr<std::pair<int,int> > fix_candidate;
            bool fix_flag = true;
            for (const auto &cord : group)
                if (field[cord] & bitvalue) {
                    if (fix_candidate) {
                        fix_flag = false;
                        break;
                    }
                    else
                        fix_candidate.reset(new std::pair<int,int>(cord));
                }

            if (!fix_candidate ||
                (fix_flag && !fix_by_bitvalue(fix_candidate->first, fix_candidate->second, bitvalue)))
                return false;
        }

        return true;
    }

    bool reduce(int row, int col, Unsigned bitvalue){
        Unsigned &tile = field.get(row,col);
        if (!(tile & bitvalue))
            return true;
        if (!(tile ^ bitvalue))
            return false;

        tile ^= bitvalue;

        return (!tile_fixed(tile) || check_reduces_from_fix(row, col, bitvalue))
        && check_fixable_by_reduction(row, col, bitvalue);
    }


    bool fix_by_bitvalue(int row, int col, Unsigned bitvalue) {
        Unsigned &tile = field.get(row,col);
        if (!(tile & bitvalue))
            return false;


        Unsigned others = tile ^ bitvalue;
        if (!others)
            return true;

        /*
        std::cout << "(" << row << "," << col <<") to " << value_of_tilevalue(bitvalue) << std::endl;
        std::cout << *this << std::endl;
         */



        do {
            // we do not use ^ because
            // although we are not sure,
            // the bitvalue in tile could be reduced

            const Unsigned other = others & -others;
            tile &= ~other;
            if(!check_fixable_by_reduction(row, col, other))
                return false;
            others = tile & ~bitvalue;
        }
        while (others);

        return tile && check_reduces_from_fix(row, col, bitvalue);
    }

    Unsigned fullbits() const {
        return ~static_cast<Unsigned>(0) >> BitSizeCalculator<Unsigned>::bitsize - getGroupN();
    }


    int value_of_tilevalue(const Unsigned &u) const {
        return
        !tile_fixed(u) ? UnfixedTile :
        (int)
        (find(bitvalue_table.begin(), bitvalue_table.end(), u)
         - bitvalue_table.begin());
    }


public:

    explicit Sudoku(int baseN_) : baseN(baseN_), field(getGroupN(),getGroupN(), fullbits())
    , bitvalue_table(getGroupN()){

        if ((int) BitSizeCalculator<Unsigned>::bitsize < getGroupN())
            throw std::invalid_argument("sudoku baseN too big");

        for (int i=0; i<getGroupN(); ++i)
            bitvalue_table[i] = (Unsigned) 1 << i;
    }


    int getGroupN() const { return baseN * baseN ; }


    int get(int row, int col) const {
        return value_of_tilevalue(field.get(row,col));
    }

    void reset() {
        for (Unsigned &tile : field)
            tile = fullbits();
    }

    bool fix(int row, int col, int value) {
        return fix_by_bitvalue(row, col, (Unsigned) 1 << value);
    }


    bool solve() {
        for (int r=0; r<getGroupN(); ++r)
            for (int c=0; c<getGroupN(); ++c) {
                Unsigned &tile = field.get(r,c);
                while (!tile_fixed(tile)) {
                    Sudoku candidate_sudoku = *this;
                    Unsigned candidate_bitvalue = tile & -tile;
                    if (candidate_sudoku.fix_by_bitvalue(r,c, candidate_bitvalue)
                        && candidate_sudoku.solve()) {
                        *this = std::move(candidate_sudoku);
                        return true;
                    }
                    if(!reduce(r, c, candidate_bitvalue))
                        return false;
                }
            }
        return true;
    }
};




template <typename Unsigned>
std::ostream &operator<<(std::ostream &os, const Sudoku<Unsigned> &sudoku) {
    const int groupN = sudoku.getGroupN();
    const int field_width
    = 1+static_cast<int>(ceil(log10(groupN)));

    for (int r=0; r<groupN; ++r) {
        for (int c=0; c<groupN; ++c)
            std::cout << std::setw(field_width) << sudoku.get(r,c);
        std::cout << std::endl;
    }

    return os;
}


template <typename Unsigned>
std::istream &operator>>(std::istream &is, Sudoku<Unsigned> &sudoku) {
    sudoku.reset();
    const int groupN = sudoku.getGroupN();

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


#endif
