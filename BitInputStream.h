//
//  BitInputStream.h
//  Huffman
//
//  Created by Xie kesong on 2/12/17.
//  Copyright © 2017 ___KesongXie___. All rights reserved.
//

#ifndef BitInputStream_h
#define BitInputStream_h

typedef unsigned char byte;

#include <iostream>

class BitInputStream{
private:
    /** one byte buffer of bits
     */
    byte buf;
    /** how many bits have been read from the buf
     */
    int nbits;
    std::istream & in;
public:
    /** constructor
     */
    BitInputStream(std::istream & is) : nbits(0), in(is){
        buf = in.get();
    };
    
    /** fill the buf from the input stream
     */
    void fill();
    
    /** read the next bit from the buf, and fill the buf from 
     *  input stream if needed.
     *  return 1 if the bit read is 1
     *  return 0 if the bit read is 0
     */
    int readBit();
    
    
    /** read a byte from istream
     */
    byte readByte();
};

#endif /* BitInputStream_h */
