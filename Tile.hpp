//
//  Tile.hpp
//  sudoku
//
//  Created by Yuki Inoue on 2013/09/08.
//  Copyright (c) 2013年 Yuki Inoue. All rights reserved.
//

#ifndef sudoku_Tile_hpp
#define sudoku_Tile_hpp

#include <bitset>
#include <vector>

template <int candidateN>
class Tile {
    std::bitset<candidateN> candidates;

public:

    Tile() {
        candidates.set();
    }

    int value() const {
        if (fixed())
            for (int i=0; i<candidateN; ++i)
                if (candidates[i])
                    return i;
        return -1;
    }

    bool fixable(int v) const {
        return candidates[v];
    }
    bool fixed() const {
        return candidates.count() == 1;
    }
    bool reducible(int v) const {
        return !candidates[v] || !fixed();
    }


    void fix(int v) {
        candidates.reset();
        candidates.set(v);
    }
    void reduce(int v) {
        candidates.reset(v);
    }


    void append_candidates(std::vector<int> &v) const {
        for (int i=0; i<candidateN; ++i)
            if (candidates[i])
                v.push_back(i);
    }
};



#endif
