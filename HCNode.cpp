//
//  HCNode.cpp
//  Huffman
//
//  Created by Xie kesong on 2/12/17.
//  Copyright © 2017 ___KesongXie___. All rights reserved.
//

#include "HCNode.h"


bool HCNode::operator<(const HCNode& other) const{
    return count > other.count;
}


