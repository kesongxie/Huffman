//
//  HCTree.cpp
//  Huffman
//
//  Created by Xie kesong on 2/12/17.
//  Copyright © 2017 ___KesongXie___. All rights reserved.
//

#include "HCTree.h"


/* descrutor should clear all the dynamic allocation*/
HCTree::~HCTree(){
    //traverse the tree and delete all the HCNodes
    //HCNode* root;
    
}

/** Use the Huffman algorithm to build a Huffman coding trie.
 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is
 *  the frequency of occurrence of byte i in the message.
 *  POSTCONDITION:  root points to the root of the trie,
 *  and leaves[i] points to the leaf node containing byte i.
 */
void HCTree::build(const vector<int>& freqs){
    
    //initialize the leaves and write it to the header of the output file
    std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> queue;

    for(unsigned i = 0; i < freqs.size(); i++){
        if(freqs[i] != 0){
            HCNode* hcNode = new HCNode(freqs[i], i);
            leaves[i] = hcNode;
            queue.push(hcNode);
        }
    }
    
    if(queue.size() == 1){
        //the queue contains one node
        HCNode* nodeFirst = queue.top();
        queue.pop();
        HCNode* parentNode = new HCNode(nodeFirst->count,nodeFirst->symbol);
        parentNode->c0 = nodeFirst;
        nodeFirst->p = parentNode;
        root = parentNode;
    }else if(queue.size() >= 2){
        while (!queue.empty() && queue.size() >= 2)
        {
            HCNode* nodeFirst = queue.top();
            queue.pop();
            HCNode* nodeSecond = queue.top();
            queue.pop();
            //create a new node as parent
            unsigned int parentCount = nodeFirst->count + nodeSecond->count;
            byte parentSymbol = max(nodeFirst->symbol, nodeSecond->symbol);
            HCNode* parentNode = new HCNode(parentCount,parentSymbol);
            
            if(nodeFirst->symbol < nodeSecond->symbol){
                parentNode->c0 = nodeFirst;
                parentNode->c1 = nodeSecond;
            }else{
                parentNode->c0 = nodeSecond;
                parentNode->c1 = nodeFirst;
            }
            nodeFirst->p = parentNode;
            nodeSecond->p = parentNode;
            queue.push(parentNode);
        }
        root = queue.top();
        queue.pop();
    }
}

/** Write to the given BitOutputStream
 *  the sequence of bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const{
    
}

/** Write to the given ofstream
 *  the sequence of bits (as ASCII) coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT
 *  BE USED IN THE FINAL SUBMISSION.
 */
void HCTree::encode(byte symbol, ofstream& out) const{
    //translate the symbol into a encoded binary string
    HCNode* node = leaves[symbol];
    vector<byte> chars;
    while(node->p != NULL){
        //check whether this is the rigth child
        if(node->p->c1 == node){
            chars.push_back('1');
        }else{
            chars.push_back('0');
        }
        node = node->p;
    }
    for(vector<byte>::reverse_iterator itr = chars.rbegin(); itr != chars.rend(); itr++){
        out << *itr;
    }
}


/** Return symbol coded in the next sequence of bits from the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 */
int HCTree::decode(BitInputStream& in) const{
    return 0;
}

/** Return the symbol coded in the next sequence of bits (represented as
 *  ASCII text) from the ifstream.
 *  PRECONDITION: build() has been called, to create the coding
 *  tree, and initialize root pointer and leaves vector.
 *  THIS METHOD IS USEFUL FOR THE CHECKPOINT BUT SHOULD NOT BE USED
 *  IN THE FINAL SUBMISSION.
 */
int HCTree::decode(ifstream& in) const{
    //construct the tree based on the header.
    //10010010101111000001111100
    HCNode* p = root;
    char c;
    while(p->c0 != NULL || p->c1 != NULL){
        //not the leaf yet
        if(in.get(c)){
            if(c == '1'){
                //go to the c1
                p = p->c1;
            }else{
                //go to the c0
                p = p->c0;
            }
        }else{
            return -1;
        }
    }
    return p->symbol;
}


/** compress file for given input file name, and output to the given destination
 *  @param inputFileName : input file name
 *  @param outputFileName
 *  @retrun bool: return true when compression executed successfully, false otherwise
 */
bool HCTree::compress(std::string inputFileName, std::string outputFileName){
    HCTree huffmanTree;
    vector<int> freq(256, 0);
    
    std::ifstream inputFile;
    inputFile.open(inputFileName);
    if(inputFile.is_open()){
        char c;
        while(inputFile.get(c)){
            freq[c] += 1;
        }
    }else{
        std::cerr << "file open failed" <<std::endl;
        return false;
    }
    
    //write header to the output file
    std::ofstream outputFile (outputFileName);
    
    for(auto counter : freq){
        outputFile << counter << "\n";
    }
    
    //build the huffman coding tree from the frequence counter
    huffmanTree.build(freq);
    
    //use the huffman coding tree to encode each byte from the input
    inputFile.clear(); //clear the eof flag
    inputFile.seekg(0, ios::beg); //point to the begining of the input file
    char c;
    while(inputFile.get(c)){
        huffmanTree.encode(c, outputFile);
    }
    inputFile.close();
    outputFile.close();
    return true;
}


/** uncompress file for given input file name, and output to the given destination
 *  @param inputFileName : name for the input file to be uncompressed
 *  @param outputFileName : output file for the uncompressed file
 *  @retrun bool: return true when uncompression executed successfully, false otherwise
 */
bool HCTree::uncompress(std::string inputFileName, std::string outputFileName){
    std::ifstream inputFile(inputFileName);
    vector<int> freq(256, 0);
    if(inputFile.is_open()){
        for(int i = 0; i < 256; i++){
            std::string line;
            getline(inputFile, line);
            freq[i] = stoi(line);
        }
        HCTree huffmanTree;
        huffmanTree.build(freq);
        std::ofstream outputFile (outputFileName);
        if(outputFile.is_open()){
            while(1){
                int c = huffmanTree.decode(inputFile);
                if(c != -1){
                   //cast to byte
                    outputFile << (byte)c;
                }else{
                    break;
                }
            }
        }else{
            std::cerr << "uncompress failed, unable to output file" << std::endl;
            return false;
        }
        outputFile.close();
    }else{
        std::cerr << "uncompress failed, can't open input file" << std::endl;
        return false;
    }
    inputFile.close();
    return true;
}









