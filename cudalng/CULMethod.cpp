//
//  CULMethod.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULMethod::~CULMethod() {
    delete type;
    for (CULParam* param : params) {
        delete param;
    }
    delete command;
}

void CULMethod::syntaxHead(CULContext *context) {
    type->syntaxHead(context);
    for (CULParam* param : params) {
        param->syntaxHead(context);
    }
    command->syntaxHead(context);
}

void CULMethod::syntaxBody(CULContext *context) {
    type->syntaxBody(context);
    for (CULParam* param : params) {
        param->syntaxBody(context);
    }
    command->syntaxBody(context);
}

void CULMethod::semantic(CULContext *context) {
    type->semantic(context);
    for (CULParam* param : params) {
        param->semantic(context);
    }
    command->semantic(context);
}