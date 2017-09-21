//
//  CULIfCommand.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULIfCommand::CULIfCommand() {
    
}

CULIfCommand::~CULIfCommand() {
    delete value;
    delete cmd;
}

void CULIfCommand::syntaxHead(CULContext *context) {
    value->syntaxHead(context);
    cmd->syntaxHead(context);
}

void CULIfCommand::syntaxBody(CULContext *context) {
    context->numberOfIf++;
    value->syntaxBody(context);
    cmd->syntaxBody(context);
}

void CULIfCommand::semantic(CULContext *context) {
    value->semantic(context);
    cmd->semantic(context);
}