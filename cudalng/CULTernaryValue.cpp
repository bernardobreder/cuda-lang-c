//
//  CULTernaryValue.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULTernaryValue::~CULTernaryValue() {
    delete center;
}

void CULTernaryValue::syntaxHead(CULContext *context) {
    CULBinaryValue::syntaxHead(context);
}

void CULTernaryValue::syntaxBody(CULContext *context) {
    CULBinaryValue::syntaxBody(context);
}

void CULTernaryValue::semantic(CULContext *context) {
    CULBinaryValue::semantic(context);
}
