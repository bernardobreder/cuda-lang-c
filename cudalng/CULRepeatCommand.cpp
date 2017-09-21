//
//  CULRepeatCommand.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULRepeatCommand::CULRepeatCommand() {
    
}

CULRepeatCommand::~CULRepeatCommand() {
    delete value;
    delete cmd;
}

void CULRepeatCommand::syntaxHead(CULContext *context) {
    value->syntaxHead(context);
    cmd->syntaxHead(context);
}

void CULRepeatCommand::syntaxBody(CULContext *context) {
    context->numberOfRepeat++;
    refreshComplexity(context);
    value->syntaxBody(context);
    cmd->syntaxBody(context);
}

void CULRepeatCommand::semantic(CULContext *context) {
    value->semantic(context);
    cmd->semantic(context);
}

bool CULRepeatCommand::isLoop() {
    return true;
}