//
//  CULDeclareCommand.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULDeclareCommand::CULDeclareCommand(){
}

CULDeclareCommand::~CULDeclareCommand() {
    if (type) delete type;
    for (CULValue *value : argValues) {
        delete value;
    }
}

void CULDeclareCommand::syntaxHead(CULContext *context) {
    for (CULValue *value : argValues) {
        value->syntaxHead(context);
    }
}

void CULDeclareCommand::syntaxBody(CULContext *context) {
    context->numberOfDeclare++;
    for (CULValue *value : argValues) {
        if (type->isShared) {
            context->sharedMemory += type->size;
        } else {
            context->globalMemory += type->size;
        }
        value->syntaxBody(context);
    }
}

void CULDeclareCommand::semantic(CULContext *context) {
    for (CULValue *value : argValues) {
        value->semantic(context);
    }
}