//
//  CULForCommand.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULForCommand::CULForCommand() {
    
}

CULForCommand::~CULForCommand() {
    delete cmd;
}

void CULForCommand::syntaxHead(CULContext *context) {
    cmd->syntaxHead(context);
}

void CULForCommand::syntaxBody(CULContext *context) {
    context->numberOfFor++;
    context->curComplexity++;
    refreshComplexity(context);
    cmd->syntaxBody(context);
    context->curComplexity--;
}

void CULForCommand::semantic(CULContext *context) {
    cmd->semantic(context);
}

bool CULForCommand::isLoop() {
    return true;
}