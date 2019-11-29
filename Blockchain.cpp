//
// Created by Charles Inwald on 2019-11-22.
//

#include "Blockchain.h"
#include <iomanip>

//Start off with a genesis block and a difficulty
Blockchain::Blockchain() {
    _vChain.emplace_back(Block(0, "Genesis Block"));
    _nDifficulty = 5;
    ifstream infile("Blockchain.dat", std::ifstream::in);

    string line;
    while( !infile.eof() )
    {
        //Get index;
        getline( infile, line );
        int index = stoi(line);
        line = "";
        cout << index << endl;
        //Get nonce
        getline( infile, line );
        int nonce = stoi(line);
        line = "";
        cout << nonce << endl;
        getline( infile, line );
        int datalength = stoi(line);
        line = "";
        cout << datalength << endl;
        //Get data
        getline( infile, line );
        string data = line;
        line = "";
        cout << data << endl;
        getline( infile, line );
        int hashlength = stoi(line);
        line = "";
        cout << hashlength << endl;
        //Get hash
        getline( infile, line );
        string hash = line;
        line = "";
        cout << hash << endl;
        //Get time
        getline( infile, line );
        long time = stol(line);
        cout << time << endl;
        line = "";
        cout << endl;
    }
    infile.close();

    file = new ofstream;
    file->open("Blockchain.dat", std::ofstream::out | std::ofstream::app);
}

//Mine a block and add it to the blockchain
void Blockchain::AddBlock(Block bNew) {
    bNew.sPrevHash = _GetLastBlock().GetHash();
    bNew.MineBlock(_nDifficulty);
    bNew.WriteBlock(*file);
    _vChain.push_back(bNew);
}

Block Blockchain::_GetLastBlock() const {
    return _vChain.back();
}

void Blockchain::DisplayBlocks() const {
    std::cout << "Blocks in chain:" << std::endl;
    for (auto i:_vChain) {
        std::cout << i.GetHash() << std::endl;
    }
}
