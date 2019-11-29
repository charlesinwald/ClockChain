//
// Created by Charles Inwald on 2019-11-22.
//

#ifndef INC_411P3_BLOCK_H
#define INC_411P3_BLOCK_H

#include <cstdint>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class Block {
public:
    //signature of hash from previous block
    string sPrevHash;

    //we use const & to pass by reference but cannot change to save
    //memory and improve efficiency
    Block(uint32_t nIndexIn, const string &sDataIn);

    string GetHash();

    void MineBlock(uint32_t nDifficulty);

    void WriteBlock(ofstream & file);

    bool writing = false;

private:
    uint32_t _nIndex;
    int64_t _nNonce;
    string _sData;
    string _sHash;
    time_t _tTime;

    //this method cannot change any of the variables in the block class which is useful for dealing
    //with a blockchain
    string _CalculateHash() const;
};

#endif //INC_411P3_BLOCK_H
