//
// Created by Charles Inwald on 2019-11-22.
//

#ifndef INC_411P3_BLOCKCHAIN_H
#define INC_411P3_BLOCKCHAIN_H
#include <cstdint>
#include <vector>
#include "Block.h"
#include <fstream>

using namespace std;

class Blockchain {
public:
    Blockchain();

    void AddBlock(Block bNew);

    void DisplayBlocks() const;

private:
    uint32_t _nDifficulty;
    vector<Block> _vChain;
    ofstream * file;
    ifstream infile;

    Block _GetLastBlock() const;

};


#endif //INC_411P3_BLOCKCHAIN_H
