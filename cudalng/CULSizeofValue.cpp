//
//  CULSizeofValue.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

void CULSizeofValue::syntaxHead(CULContext *context) {
    CULValue::syntaxHead(context);
    type->syntaxHead(context);
}

void CULSizeofValue::syntaxBody(CULContext *context) {
    CULValue::syntaxBody(context);
    type->syntaxBody(context);
}

void CULSizeofValue::semantic(CULContext *context) {
    CULValue::semantic(context);
    type->semantic(context);
}
