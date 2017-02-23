//
//  compress.cpp
//  Huffman
//
//  Created by Xie kesong on 2/12/17.
//  Copyright © 2017 ___KesongXie___. All rights reserved.
//

#include <iostream>
#include <string>
#include "HCTree.h"
#include "BitOutputStream.h"

int main(int argc, char** argv){
    if(argc < 3){
        std::cerr << "In correct usage, uncompress program requries two arguments, inputFileName and outputFileName" << std::endl;
        return -1;
    }
    string inputFileName = *(++argv);
    string outputFileName = *(++argv);
    HCTree huffmanTree;
    huffmanTree.compress(inputFileName, outputFileName);
    return 0;
}

