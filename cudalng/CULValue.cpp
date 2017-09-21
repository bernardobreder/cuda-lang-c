//
//  CULValue.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

bool CULValue::assignable() {
    return true;
}

void CULValue::syntaxHead(CULContext *context) {
    CULNode::syntaxHead(context);
}

void CULValue::syntaxBody(CULContext *context) {
    CULNode::syntaxBody(context);
}

void CULValue::semantic(CULContext *context) {
    CULNode::semantic(context);
}