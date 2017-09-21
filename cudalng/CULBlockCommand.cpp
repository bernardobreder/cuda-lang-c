//
//  CULBlockCommand.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULBlockCommand::CULBlockCommand() {
    
}

CULBlockCommand::~CULBlockCommand() {
    for (CULCommand* cmd : cmds) {
        delete cmd;
    }
}

void CULBlockCommand::syntaxHead(CULContext *context) {
    for (CULCommand* cmd : cmds) {
        cmd->syntaxHead(context);
    }
}

void CULBlockCommand::syntaxBody(CULContext *context) {
    for (CULCommand* cmd : cmds) {
        cmd->syntaxBody(context);
    }
}

void CULBlockCommand::semantic(CULContext *context) {
    for (CULCommand* cmd : cmds) {
        cmd->semantic(context);
    }
}