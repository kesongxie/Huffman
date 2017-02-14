//
//  uncompress.cpp
//  Huffman
//
//  Created by Xie kesong on 2/13/17.
//  Copyright © 2017 ___KesongXie___. All rights reserved.
//
//
#include <iostream>
#include <string>
#include "HCTree.h"

int main(int argc, char** argv){
    if(argc < 3){
        std::cerr << "In correct usage, uncompress program requries two arguments, inputFileName and outputFileName" << std::endl;
        return -1;
    }
    
    string inputFileName = *(++argv);
    std::cout << inputFileName << std::endl;
    string outputFileName = *(++argv);
    std::cout << outputFileName << std::endl;
    HCTree::uncompress(inputFileName, outputFileName);
    return 0;
}
