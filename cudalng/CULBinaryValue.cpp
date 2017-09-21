//
//  CULBinaryValue.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULBinaryValue::~CULBinaryValue() {
    delete right;
}

void CULBinaryValue::syntaxHead(CULContext *context) {
    CULUnaryValue::syntaxHead(context);
    right->syntaxHead(context);
}

void CULBinaryValue::syntaxBody(CULContext *context) {
    CULUnaryValue::syntaxBody(context);
    right->syntaxBody(context);
}

void CULBinaryValue::semantic(CULContext *context) {
    CULUnaryValue::semantic(context);
    right->semantic(context);
}
