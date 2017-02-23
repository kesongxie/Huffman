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
    std::cout << inputFileName << std::endl;
    string outputFileName = *(++argv);
    std::cout << outputFileName << std::endl;
    
//    std::string inputFileName = "/Users/KesongXie/Developer/CSE100/Huffman copy/Huffman/checkpoint1.txt";
//    std::string outputFileName = "/Users/KesongXie/Developer/CSE100/Huffman copy/Huffmanheck1.comp.txt";

    
    HCTree huffmanTree;
    huffmanTree.compress(inputFileName, outputFileName);
    
            
    return 0;
}

