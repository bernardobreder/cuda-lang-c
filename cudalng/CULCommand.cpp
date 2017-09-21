//
//  NULCommand.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULCommand::CULCommand() {
    
}

CULCommand::~CULCommand() {
    
}

bool CULCommand::isLoop() {
    return false;
}

void CULCommand::refreshComplexity(CULContext *context) {
    int parentLoopCount = 0;
    CULCommand *cmd = this;
    while (cmd) {
        if (cmd->isLoop()) {
            parentLoopCount++;
        }
        cmd = cmd->parent;
    }
    if (parentLoopCount > context->maxComplexity) {
        context->maxComplexity = parentLoopCount;
    }
}