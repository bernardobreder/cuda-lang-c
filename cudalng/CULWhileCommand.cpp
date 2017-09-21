//
//  CULWhileCommand.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULWhileCommand::CULWhileCommand() {
    
}

CULWhileCommand::~CULWhileCommand() {
    delete value;
    delete cmd;
}

void CULWhileCommand::syntaxHead(CULContext *context) {
    value->syntaxHead(context);
    cmd->syntaxHead(context);
}

void CULWhileCommand::syntaxBody(CULContext *context) {
    context->numberOfWhile++;
    refreshComplexity(context);
    value->syntaxBody(context);
    cmd->syntaxBody(context);
}

void CULWhileCommand::semantic(CULContext *context) {
    value->semantic(context);
    cmd->semantic(context);
}

bool CULWhileCommand::isLoop() {
    return true;
}