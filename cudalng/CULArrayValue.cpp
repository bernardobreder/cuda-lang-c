//
//  CULArrayValue.cpp
//  cudalng
//
//  Created by Bernardo Breder on 17/06/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULArrayValue::~CULArrayValue() {
    delete left;
    delete right;
}