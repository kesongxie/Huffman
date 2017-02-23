//
//  BitInputStream.cpp
//  Huffman
//
//  Created by Xie kesong on 2/12/17.
//  Copyright © 2017 ___KesongXie___. All rights reserved.
//

#include "BitInputStream.h"

/** fill the buf from the input stream
 */
void BitInputStream::fill(){
    buf = in.get();
    nbits = 0;
}

/** read the next bit from the buf, and fill the buf from
 *  input stream if needed.
 *  return 1 if the bit read is 1
 *  return 0 if the bit read is 0
 */
int BitInputStream::readBit(){
    if(nbits == 8){
        fill();
    }
    int bit = ((~(1 << (7 - nbits)) | buf) >> (7 - nbits) & 1);
    nbits++;
    return bit;
}

/** read a byte from istream
 */
byte BitInputStream::readByte(){
    byte c = 0;
    for(int i = 7; i >=0; i--){
        c |= (readBit() << i);
    }
    
    return c;
}

