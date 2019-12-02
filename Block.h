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
    int senderid;

    //we use const & to pass by reference but cannot change to save
    //memory and improve efficiency
    Block(uint32_t nIndexIn, const string &sDataIn);

    Block(uint32_t nIndexIn, const string &sDataIn, string hash, time_t time, int sender);

    string GetHash();

    int GetSender();

    void MineBlock(uint32_t nDifficulty);

    void WriteBlock(ofstream & file);


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
