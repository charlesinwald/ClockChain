//
// Created by Charles Inwald on 2019-11-22.
//

#include "Block.h"
#include "sha256.h"
#include <unistd.h>

//Constructor for mining
Block::Block(uint32_t nIndexIn, const string &sDataIn) : _nIndex(nIndexIn), _sData(sDataIn) {
    //arbitrary number to be used once in the cryptography
    _nNonce = -1;
    //set to current time
    _tTime = time(nullptr);
    senderid = 0;
}

//Constructor for reading in
Block::Block(uint32_t nIndexIn, const string &sDataIn, string hash, time_t time, int sender) {
    _nIndex = nIndexIn;
    _sData = sDataIn;
    _sHash = hash;
    _tTime = time;
    senderid = sender;
}

//simple getter method
string Block::GetHash() {
    return _sHash;
}

int Block::GetSender() {
    return senderid;
}

void Block::MineBlock(uint32_t nDifficulty) {
    //create an array of characters with length one more than nDifficulty
    char cstr[nDifficulty + 1];
    //fill it with zeroes
    for (uint32_t i = 0; i < nDifficulty; ++i) {
        cstr[i] = '0';
    }
    //null terminate
    cstr[nDifficulty] = '\0';
    //convert to C++ string
    string str(cstr);

    do {
        _nNonce++;
        _sHash = _CalculateHash();
    } while (_sHash.substr(0, nDifficulty) != str);


    //Server timer
    sleep(60);

}

/*
 * We need to write the members to a file, as well as the sizes of the variable length members for proper serialization
 */
void Block::WriteBlock(ofstream & file) {
    file << _nIndex << endl;
    file << _nNonce << endl;
    file << sizeof(_sData) << endl;
    file << _sData << endl;
    file << sizeof(_sHash) << endl;
    file << _sHash << endl;
    file << _tTime << endl;
//    cout << senderid << endl;
    file << senderid << endl;
}

inline string Block::_CalculateHash() const {
    //perform sha256 on the index,time,data,nonce and previous hash
    stringstream ss;
    ss << _nIndex << _tTime << _sData << _nNonce << sPrevHash;

    return sha256(ss.str());
}