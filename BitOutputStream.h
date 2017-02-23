//
//  BitOutputStream.h
//  Huffman
//
//  Created by Xie kesong on 2/12/17.
//  Copyright © 2017 ___KesongXie___. All rights reserved.
//

#ifndef BitOutputStream_h
#define BitOutputStream_h

#include <iostream>
typedef unsigned char byte;

class BitOutputStream {
private:
    /** one byte buffer of bits
     */
    byte buf;
    /** how many bits have been written to buf
     */
    int nbits;
    
    /** reference to the ouput stream to use
     */
    std::ostream & out;
        
public:
    /** constructor, clear buf and bit counter, initialize the out
     */
    BitOutputStream(std::ostream & os) : buf(0),  nbits(0), out(os){};
    
    /** writes the remaining nbits to the ostream out
     */
    ~BitOutputStream();
    
    /** send buffer to the output, and clear it
     */
    void flush();
    
    /** write the least significant of the argument to the buffer. If 
     *  the buffer is not full, increment the counter and write a bit to the
     *  buffer. Otherwise, flush the buffer first and proceed.
     */
    void writeBit(int i);
    
    /** write a byte to the ostream
     */
    void writeByte(byte c);

    
};

#endif /* BitOutputStream_h */
