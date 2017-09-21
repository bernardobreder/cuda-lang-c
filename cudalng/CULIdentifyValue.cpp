//
//  CULIdentifyValue.cpp
//  cudalng
//
//  Created by Bernardo Breder on 27/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

void CULIdentifyValue::syntaxHead(CULContext *context) {
    CULValue::syntaxHead(context);
}

void CULIdentifyValue::syntaxBody(CULContext *context) {
    CULValue::syntaxBody(context);
    if (token.length() == 13 && !strncmp(token.word(), "__syncthreads", 13)) {
        context->numberSync++;
    }
}

void CULIdentifyValue::semantic(CULContext *context) {
    CULValue::semantic(context);
}
