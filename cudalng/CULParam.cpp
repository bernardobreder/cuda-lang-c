//
//  CULParam.cpp
//  cudalng
//
//  Created by Bernardo Breder on 17/06/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULParam::~CULParam() {
    if (type) delete type;
}

void CULParam::syntaxHead(CULContext *context) {
    type->syntaxHead(context);
}

void CULParam::syntaxBody(CULContext *context) {
    type->syntaxBody(context);
}

void CULParam::semantic(CULContext *context) {
    type->semantic(context);
}