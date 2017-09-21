//
//  CULExpressionCommand.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULValueCommand::CULValueCommand(CULValue *value) {
    _value = value;
}

CULValueCommand::~CULValueCommand() {
    delete _value;
}

void CULValueCommand::syntaxHead(CULContext *context) {
    _value->syntaxHead(context);
}

void CULValueCommand::syntaxBody(CULContext *context) {
    _value->syntaxBody(context);
}

void CULValueCommand::semantic(CULContext *context) {
    _value->semantic(context);
}