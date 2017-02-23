//
//  BitOutputStream.cpp
//  Huffman
//
//  Created by Xie kesong on 2/12/17.
//  Copyright © 2017 ___KesongXie___. All rights reserved.
//

#include "BitOutputStream.h"


/** writes the remaining nbits to the ostream and recording ending nbits position
 */
BitOutputStream::~BitOutputStream(){
    flush();
}

/** send buffer to the output, and clear it
 */
void BitOutputStream::flush(){
    out.put(buf);
    out.flush();
    nbits = buf = 0;
}


/** write the least significant of the argument to the buffer. If
 *  the buffer is not full, increment the counter and write a bit to the
 *  buffer. Otherwise, flush the buffer first and then proceed.
 */
void BitOutputStream::writeBit(int i){
    //flush when the buffer is full
    if(nbits == 8){
        flush();
    }
    buf = (buf | (i << (7 - nbits)));
    nbits++;
}

/** write a byte to the ostream
 */
void BitOutputStream::writeByte(byte c){
    for(int i = 7; i >= 0; i--){
        byte p = ((~(1 << i) | c ) >> i & 1);
        int val = p;
        writeBit(val);
    }
}





