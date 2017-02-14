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

int main(int argc, char** argv){
    string inputFileName = "/Users/KesongXie/Developer/CSE100/Huffman/Huffman/checkpoint1.txt";
    string outputFileName= "/Users/KesongXie/Developer/CSE100/Huffman/Huffman/checkpoint1_compressed.txt";
    HCTree::compress(inputFileName, outputFileName);
    
    string inputFileNameSecond = "/Users/KesongXie/Developer/CSE100/Huffman/Huffman/checkpoint2.txt";
    string outputFileNameSecond= "/Users/KesongXie/Developer/CSE100/Huffman/Huffman/checkpoint2_compressed.txt";
    HCTree::compress(inputFileName, outputFileName);
    HCTree::compress(inputFileNameSecond, outputFileNameSecond);

    return 0;
}

