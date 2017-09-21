//
//  CULGetValue.cpp
//  cudalng
//
//  Created by Bernardo Breder on 17/06/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULGetValue::~CULGetValue() {
    delete left;
    delete right;
}

void CULGetValue::syntaxHead(CULContext *context) {
    CULValue::syntaxHead(context);
    left->syntaxHead(context);
    right->syntaxHead(context);
}

void CULGetValue::syntaxBody(CULContext *context) {
    CULValue::syntaxBody(context);
    left->syntaxBody(context);
    right->syntaxBody(context);
}

void CULGetValue::semantic(CULContext *context) {
    CULValue::semantic(context);
    left->semantic(context);
    right->semantic(context);
}