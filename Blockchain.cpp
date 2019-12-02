//
// Created by Charles Inwald on 2019-11-22.
//

#include "Blockchain.h"
#include <iomanip>
//Start off with a genesis block and a difficulty
Blockchain::Blockchain() {
    _vChain.emplace_back(Block(0, "Genesis Block"));
    _nDifficulty = 5;
    LoadBlocks();
    file = new ofstream;
    file->open("Blockchain.dat", std::ofstream::out | std::ofstream::app);
}

//Mine a block and add it to the blockchain
void Blockchain::AddBlock(Block bNew) {
    bNew.sPrevHash = _GetLastBlock().GetHash();
    bNew.MineBlock(_nDifficulty);
    //Must set a writing flag so we don't quit during a partially written block
    writing = true;
    bNew.WriteBlock(*file);
    writing = false;
    _vChain.push_back(bNew);
}

//Read in previous blockchain from file
void Blockchain::LoadBlocks(){
    ifstream infile("Blockchain.dat", std::ifstream::in);
    //if file opened successfully and is not empty
    if (infile.good() && (infile.peek() != std::ifstream::traits_type::eof())) {
        string line;
        cout << "Reading in previous Blockchain" << endl;
        //Read line by line, loading in data
        while (!infile.eof()) {
            //Peek to make sure the id is actually there
            //to avoid segfaults
            int c = infile.peek();
            //Abort if EOF or not a digit or empty string
            if (!isdigit(c) || (c == EOF) || line.size()) {
                break;
            }
            //Get index;
            getline(infile, line);
            int index = stoi(line);
            line = "";
            //Get nonce
            getline(infile, line);
//            int nonce = stoi(line);
            line = "";
            getline(infile, line);
//            int datalength = stoi(line);
            line = "";
            //Get data
            getline(infile, line);
            string data = line;
            line = "";
            getline(infile, line);
//            int hashlength = stoi(line);
            line = "";
            //Get hash
            getline(infile, line);
            string hash = line;
            line = "";
            //Get time
            getline(infile, line);
            long time = stol(line);
            line = "";
            Block bNew = Block(index,data,hash,time);
            _vChain.push_back(bNew);
        }
        infile.close();
        cout << "Loading complete" << endl;
    }
}

Block Blockchain::_GetLastBlock() const {
    return _vChain.back();
}

//Display block hashes one by one
void Blockchain::DisplayBlocks() const {
    std::cout << endl << "Blocks in chain:" << std::endl;
    int j = 0;
    for (auto i:_vChain) {
        if (j!=0) {
            std::cout << "Block " << j << ": " << i.GetHash() << std::endl;
        }
        j++;
    }
}
