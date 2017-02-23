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
    deallocateAux(root);
}

void HCTree::deallocateAux(HCNode* p){
    if(p != NULL){
        deallocateAux(p->c0);
        deallocateAux(p->c1);
        delete p;
    }
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
    distinctSymbolSize = (unsigned int)queue.size();
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
    //translate the symbol into a encoded binary string
    HCNode* node = leaves[symbol];
    vector<int> chars;
    while(node->p != NULL){
        //check whether this is the rigth child
        if(node->p->c1 == node){
            chars.push_back(1);
        }else{
            chars.push_back(0);
        }
        node = node->p;
    }
    for(vector<int>::reverse_iterator itr = chars.rbegin(); itr != chars.rend(); itr++){
        out.writeBit(*itr);
    }
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
    //construct the tree based on the header.
    HCNode* p = root;
    while(p->c0 != NULL || p->c1 != NULL){
        //not the leaf yet
        int bit = in.readBit();
        if(bit == 1){
            //go to the c1
            p = p->c1;
        }else{
            //go to the c0
            p = p->c0;
        }
    }
    return p->symbol;

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
 *  @param outputFileName : the file where all the encoded data go
 *  @retrun bool: return true when compression executed successfully, false otherwise
 */
bool HCTree::compress(std::string inputFileName, std::string outputFileName){
    
    //read the total of bytes needed to be encoded
    std::ifstream readStream;
    readStream.open(inputFileName);
    readStream.seekg(0, ios::end);
    long long totalBytes = readStream.tellg();
    readStream.close();
    
    //write the total of bytes needed to be encoded to the output file
    std::ofstream writeStream;
    writeStream.open(outputFileName);
    writeStream.write((char *) & totalBytes, sizeof(long long));
    
    
    //build the huffman coding tree from the frequence counter
    vector<int> freq(256, 0);
    std::ifstream inputFile;
    inputFile.open(inputFileName);
    if(inputFile.is_open()){
        char c;
        while(inputFile.get(c)){
            freq[c] += 1;
        }
    }else{
        std::cerr << "file open failed" << std::endl;
        return false;
    }
    inputFile.close();
    
    //build the coding tree
    build(freq);
    
    //write the distinct symbol size to the header
    std::cout << "distinct symbol size: " << distinctSymbolSize << std::endl;
    writeStream.write((char *) & distinctSymbolSize, sizeof(unsigned int));
    
    //encode the actual tree structure
    BitOutputStream bitOutputStream(writeStream);
    encodeTreeStructure(root, bitOutputStream);
    
    

    inputFile.open(inputFileName);

    char c;
    while(inputFile.get(c)){
        encode(c, bitOutputStream);
    }
    
    return false;
}


/** uncompress file for given input file name, and output to the given destination
 *  @param inputFileName : name for the input file to be uncompressed
 *  @param outputFileName : output file for the uncompressed file
 *  @retrun bool: return true when uncompression executed successfully, false otherwise
 */
bool HCTree::uncompress(std::string inputFileName, std::string outputFileName){
    
    //read the total of bytes form the encoded file
    std::ifstream inStream;
    inStream.open(inputFileName);
    long long totalBytesEncoded; //total bytes encoded
    inStream.read((char * ) & totalBytesEncoded, sizeof(long long));
    
    unsigned int uniqueSymbolSize; //the unique symbol size
    inStream.read((char * ) & uniqueSymbolSize, sizeof(unsigned int));
    
    BitInputStream bitInputStream(inStream);
    
    //re-construct the coding tree from the header
    root = buildTreeFromHeader(bitInputStream, uniqueSymbolSize);
    
    std::ofstream outputFile;
    outputFile.open(outputFileName);
    
    long long byteRetrived = 0;
    while(byteRetrived < totalBytesEncoded){
        byte c = decode(bitInputStream);
       //cast to byte
        outputFile << c ;
        byteRetrived++;
    }
    inStream.close();
    outputFile.close();
    return true;
}


/** encode the encoded file header with the tree structure
 */
void HCTree::encodeTreeStructure(HCNode* node, BitOutputStream & bitOutputStream){
    if(node != NULL){
        if(node->c0 == NULL && node->c1 == NULL){
            //this the a leaf node
            bitOutputStream.writeBit(1);
            bitOutputStream.writeByte(node->symbol);
        }else{
            bitOutputStream.writeBit(0);
            encodeTreeStructure(node->c0, bitOutputStream);
            encodeTreeStructure(node->c1, bitOutputStream);
        }
    }
}


/** build the huffman coding tree from the header
 */
HCNode* HCTree::buildTreeFromHeader(BitInputStream & inStream, unsigned int & byteRemaining){
    if(byteRemaining > 0){
        //create the root node
        //01 ___A____ 01 ___ B ____ 1 ___C____ 0000
        int bit = inStream.readBit();
        if(bit == 1){
            //the next byte is the leaf symbol
            byte symbol = inStream.readByte();
            byteRemaining--;
            return new HCNode(0, symbol);
        }else{
            //internal node
            HCNode* internal = new HCNode(0, 0);
            internal->c0 = buildTreeFromHeader(inStream, byteRemaining);
            internal->c1 = buildTreeFromHeader(inStream, byteRemaining);
            return internal;
        }
    }
    return NULL;
}


void HCTree::inOrder(HCNode* p){
    if(p != NULL){
        if(p->symbol == 0){
            std::cout << "interna;" << std::endl;
        }else{
            std::cout << p->symbol << std::endl;
        }
        inOrder(p->c0);
        inOrder(p->c1);
    }
}
















