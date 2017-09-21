//
//  CULToken.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULToken::CULToken(const char* word, long length, unsigned char type) {
    _word = word;
    _len = length;
    _type = type;
}

CULToken::CULToken() {
    _word = 0;
    _len = 0;
    _type = 0;
}

unsigned char CULToken::type() {
    return _type;
}

const char* CULToken::word() {
    return _word;
}

long CULToken::length() {
    return _len;
}

bool CULToken::eof() {
    return _type == 0;
}