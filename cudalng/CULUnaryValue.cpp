//
//  CULUnaryValue.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULUnaryValue::~CULUnaryValue() {
    delete left;
}

void CULUnaryValue::syntaxHead(CULContext *context) {
    CULValue::syntaxHead(context);
    left->syntaxHead(context);
}

void CULUnaryValue::syntaxBody(CULContext *context) {
    CULValue::syntaxBody(context);
    left->syntaxBody(context);
}

void CULUnaryValue::semantic(CULContext *context) {
    CULValue::semantic(context);
    left->semantic(context);
}
