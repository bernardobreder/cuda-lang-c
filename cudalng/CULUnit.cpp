//
//  CULUnit.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULUnit::~CULUnit() {
    for (CULNode* node : nodes) {
        delete node;
    }
}

void CULUnit::syntaxHead(CULContext *context) {
    for (CULNode* node : nodes) {
        node->syntaxHead(context);
    }
}

void CULUnit::syntaxBody(CULContext *context) {
    for (CULNode* node : nodes) {
        node->syntaxBody(context);
    }
}

void CULUnit::semantic(CULContext *context) {
    for (CULNode* node : nodes) {
        node->semantic(context);
    }
}