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
    string inputFileName = "/Users/KesongXie/Developer/CSE100/Huffman/Huffman/checkpoint1_compressed.txt";
    string outputFileName= "/Users/KesongXie/Developer/CSE100/Huffman/Huffman/checkpoint1_uncompressed.txt";
    string inputFileNameSecond = "/Users/KesongXie/Developer/CSE100/Huffman/Huffman/checkpoint2_compressed.txt";
    string outputFileNameSecond= "/Users/KesongXie/Developer/CSE100/Huffman/Huffman/checkpoint2_uncompressed.txt";
    HCTree::uncompress(inputFileName, outputFileName);
    HCTree::uncompress(inputFileNameSecond, outputFileNameSecond);
    return 0;
}
