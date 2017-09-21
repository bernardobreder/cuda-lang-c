//
//  Lexical.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULLexical::CULLexical(const char* text) : _next(text, 0, 01), _eof(0, 0, 0) {
    _tokens.clear();
    _index = 0;
    char *_cur = (char*) text;
    CULToken *token;
    char* word;
    long type, length;
    while (*_cur) {
        token = 0;
        char c = *_cur;
        while (c <= 32) {
            if (c <= 0) return;
            c = *(++_cur);
        }
        char c2 = _cur[1];
        char c3 = _cur[2];
        if (c == '\"') {
            c = *(++_cur);
            while (c != '\"') {
                c = *(++_cur);
            }
            _cur++;
        } else if (c == '\'') {
            c = *(++_cur);
            _cur++;
        } else if (c == '/' && c2 == '*') {
            _cur += 2;
            c = _cur[0];
            c = _cur[1];
            while (c != '*' || c2 != '/') {
                _cur++;
                c = _cur[0];
                c2 = _cur[1];
            }
            _cur += 2;
        } else if (c == '/' && c2 == '/') {
            _cur += 2;
            c = *_cur;
            while (c != '\n') {
                c = *(++_cur);
            }
            _cur++;
        } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$') {
            word = _cur;
            c = *(++_cur);
            while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$') {
                c = *(++_cur);
            }
            type = CUL_LEXICAL_ID;
            length = _cur - word;
            switch (length) {
                case 2: {
                    switch (word[0]) {
                        case 'd':{
                            if (word[1] == 'o') type = CUL_LEXICAL_DO;
                            break;
                        }
                        case 'i':{
                            if (word[1] == 'f') type = CUL_LEXICAL_IF;
                            break;
                        }
                    }
                    break;
                }
                case 3: {
                    switch (word[0]) {
                        case 'f':{
                            if (word[1] == 'o' && word[2] == 'r') type = CUL_LEXICAL_FOR;
                            break;
                        }
                        case 'i':{
                            if (word[1] == 'n' && word[2] == 't') type = CUL_LEXICAL_INT;
                            break;
                        }
                    }
                    break;
                }
                case 4: {
                    switch (word[0]) {
                        case 'a':{
                            if (word[1] == 'u' && word[2] == 't' && word[3] == 'o') type = CUL_LEXICAL_AUTO;
                            break;
                        }
                        case 'c':{
                            if (word[1] == 'h' && word[2] == 'a' && word[3] == 'r') type = CUL_LEXICAL_CHAR;
                            break;
                        }
                        case 'e':{
                            if (word[1] == 'l' && word[2] == 's' && word[3] == 'e') type = CUL_LEXICAL_ELSE;
                            else if (word[1] == 'n' && word[2] == 'u' && word[3] == 'm') type = CUL_LEXICAL_ENUM;
                            break;
                        }
                        case 'g':{
                            if (word[1] == 'o' && word[2] == 't' && word[3] == 'o') type = CUL_LEXICAL_GOTO;
                            break;
                        }
                        case 'l':{
                            if (word[1] == 'o' && word[2] == 'n' && word[3] == 'g') type = CUL_LEXICAL_LONG;
                            break;
                        }
                        case 't':{
                            if (word[1] == 'r' && word[2] == 'u' && word[3] == 'e') type = CUL_LEXICAL_TRUE;
                            break;
                        }
                        case 'v':{
                            if (word[1] == 'o' && word[2] == 'i' && word[3] == 'd') type = CUL_LEXICAL_VOID;
                            break;
                        }
                    }
                    break;
                }
                case 5: {
                    switch (word[0]) {
                        case 'b':{
                            if (word[1] == 'r' && word[2] == 'e' && word[3] == 'a' && word[4] == 'k') type = CUL_LEXICAL_BREAK;
                            break;
                        }
                        case 'c':{
                            if (word[1] == 'o' && word[2] == 'n' && word[3] == 's' && word[4] == 't') type = CUL_LEXICAL_CONST;
                            break;
                        }
                        case 'f':{
                            if (word[1] == 'a' && word[2] == 'l' && word[3] == 's' && word[4] == 'e') type = CUL_LEXICAL_FALSE;
                            else if (word[1] == 'l' && word[2] == 'o' && word[3] == 'a' && word[4] == 't') type = CUL_LEXICAL_FLOAT;
                            break;
                        }
                        case 's':{
                            if (word[1] == 'h' && word[2] == 'o' && word[3] == 'r' && word[4] == 't') type = CUL_LEXICAL_SHORT;
                            break;
                        }
                        case 'u':{
                            if (word[1] == 'n' && word[2] == 'i' && word[3] == 'o' && word[4] == 'n') type = CUL_LEXICAL_UNION;
                            break;
                        }
                        case 'w':{
                            if (word[1] == 'h' && word[2] == 'i' && word[3] == 'l' && word[4] == 'e') type = CUL_LEXICAL_WHILE;
                            break;
                        }
                    }
                    break;
                }
                case 6: {
                    switch (word[0]) {
                        case 'd':{
                            if (word[1] == 'o' && word[2] == 'u' && word[3] == 'b' && word[4] == 'l' && word[5] == 'e') type = CUL_LEXICAL_DOUBLE;
                            break;
                        }
                        case 'e':{
                            if (word[1] == 'x' && word[2] == 't' && word[3] == 'e' && word[4] == 'r' && word[5] == 'n') type = CUL_LEXICAL_EXTERN;
                            break;
                        }
                        case 'r':{
                            if (word[1] == 'e' && word[2] == 't' && word[3] == 'u' && word[4] == 'r' && word[5] == 'n') type = CUL_LEXICAL_RETURN;
                            break;
                        }
                        case 's':{
                            if (word[1] == 'i' && word[2] == 'z' && word[3] == 'e' && word[4] == 'o' && word[5] == 'f') type = CUL_LEXICAL_SIZEOF;
                            else if (word[1] == 't' && word[2] == 'a' && word[3] == 't' && word[4] == 'i' && word[5] == 'c') type = CUL_LEXICAL_STATIC;
                            else if (word[1] == 't' && word[2] == 'r' && word[3] == 'u' && word[4] == 'c' && word[5] == 't') type = CUL_LEXICAL_STRUCT;
                            else if (word[1] == 'w' && word[2] == 'i' && word[3] == 't' && word[4] == 'c' && word[5] == 'h') type = CUL_LEXICAL_SWITCH;
                            break;
                        }
                    }
                    break;
                }
                case 7: {
                    switch (word[0]) {
                        case 't':{
                            if (word[1] == 'y' && word[2] == 'p' && word[3] == 'e' && word[4] == 'd' && word[5] == 'e' && word[6] == 'f') type = CUL_LEXICAL_TYPEDEF;
                            break;
                        }
                    }
                    break;
                }
                case 8: {
                    switch (word[0]) {
                        case 'c':{
                            if (word[1] == 'o' && word[2] == 'n' && word[3] == 't' && word[4] == 'i' && word[5] == 'n' && word[6] == 'u' && word[7] == 'e') type = CUL_LEXICAL_TYPEDEF;
                            break;
                        }
                        case 'r':{
                            if (word[1] == 'e' && word[2] == 'g' && word[3] == 'i' && word[4] == 's' && word[5] == 't' && word[6] == 'e' && word[7] == 'r') type = CUL_LEXICAL_TYPEDEF;
                            break;
                        }
                        case 'u':{
                            if (word[1] == 'n' && word[2] == 's' && word[3] == 'i' && word[4] == 'g' && word[5] == 'n' && word[6] == 'e' && word[7] == 'd') type = CUL_LEXICAL_TYPEDEF;
                            break;
                        }
                        case 'v':{
                            if (word[1] == 'o' && word[2] == 'l' && word[3] == 'a' && word[4] == 't' && word[5] == 'i' && word[6] == 'l' && word[7] == 'e') type = CUL_LEXICAL_TYPEDEF;
                            break;
                        }
                    }
                    break;
                }
                case 10: {
                    switch (word[0]) {
                        case '_':{
                            if (word[1] == '_' && word[2] == 'd' && word[3] == 'e' && word[4] == 'v' && word[5] == 'i' && word[6] == 'c' && word[7] == 'e' && word[8] == '_' && word[9] == '_') type = CUL_LEXICAL_DEVICE;
                            else if (word[1] == '_' && word[2] == 'g' && word[3] == 'l' && word[4] == 'o' && word[5] == 'b' && word[6] == 'a' && word[7] == 'l' && word[8] == '_' && word[9] == '_') type = CUL_LEXICAL_GLOBAL;
                            else if (word[1] == '_' && word[2] == 'k' && word[3] == 'e' && word[4] == 'r' && word[5] == 'n' && word[6] == 'e' && word[7] == 'l' && word[8] == '_' && word[9] == '_') type = CUL_LEXICAL_KERNEL;
                            else if (word[1] == '_' && word[2] == 's' && word[3] == 'h' && word[4] == 'a' && word[5] == 'r' && word[6] == 'e' && word[7] == 'd' && word[8] == '_' && word[9] == '_') type = CUL_LEXICAL_SHARED;
                            break;
                        }
                    }
                    break;
                }
            }
            token = new CULToken(word, length, type);
        } else if ((c >= '0' && c <= '9') || (c == '.' && c2 >= '0' && c2 <= '9')) {
            word = _cur;
            if (c == '0' && c2 == 'x') {
                _cur += 2;
                c = *_cur;
                while (c >= '0' && c <= '9') {
                    c = *(++_cur);
                }
            } else {
                c = *(++_cur);
                bool dot = false;
                while ((c >= '0' && c <= '9') || c == '.') {
                    if (c == '.') {
                        if (dot) break;
                        dot = true;
                    }
                    c = *(++_cur);
                }
            }
            type = CUL_LEXICAL_NUMBER;
            length = _cur - word;
            token = new CULToken(word, length, type);
        } else {
            word = _cur;
            type = *(_cur++);
            length = 1;
            switch (c) {
                case '.': {
                    switch (c2) {
                        case '.': {
                            switch (c3) {
                                case '.':
                                    type = CUL_LEXICAL_DOT3;
                                    length = 3;
                                    _cur += 2;
                                    break;
                            }
                            break;
                        }
                    }
                    break;
                }
                case '=': {
                    switch (c2) {
                        case '=': {
                            type = CUL_LEXICAL_EQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                    }
                    break;
                }
                case '!': {
                    switch (c2) {
                        case '=': {
                            type = CUL_LEXICAL_NOTEQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                    }
                    break;
                }
                case '+': {
                    switch (c2) {
                        case '+': {
                            type = CUL_LEXICAL_SUMSUM;
                            length = 2;
                            _cur++;
                            break;
                        }
                        case '=': {
                            type = CUL_LEXICAL_SUM_EQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                    }
                    break;
                }
                case '-': {
                    switch (c2) {
                        case '-': {
                            type = CUL_LEXICAL_SUBSUB;
                            length = 2;
                            _cur++;
                            break;
                        }
                        case '=': {
                            type = CUL_LEXICAL_SUB_EQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                        case '>': {
                            type = CUL_LEXICAL_POINTER;
                            length = 2;
                            _cur++;
                            break;
                        }
                    }
                    break;
                }
                case '*': {
                    switch (c2) {
                        case '=': {
                            type = CUL_LEXICAL_MUL_EQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                    }
                    break;
                }
                case '/': {
                    switch (c2) {
                        case '=': {
                            type = CUL_LEXICAL_DIV_EQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                    }
                    break;
                }
                case '>': {
                    switch (c2) {
                        case '=': {
                            type = CUL_LEXICAL_GREATER_EQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                        case '>': {
                            type = CUL_LEXICAL_SHIFT_RIGHT;
                            length = 2;
                            _cur++;
                            switch (c3) {
                                case '=': {
                                    type = CUL_LEXICAL_SHIFT_RIGHT_EQUAL;
                                    length = 3;
                                    _cur++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case '<': {
                    switch (c2) {
                        case '=': {
                            type = CUL_LEXICAL_LOWER_EQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                        case '<': {
                            type = CUL_LEXICAL_SHIFT_LEFT;
                            length = 2;
                            _cur++;
                            switch (c3) {
                                case '=': {
                                    type = CUL_LEXICAL_SHIFT_LEFT_EQUAL;
                                    length = 3;
                                    _cur++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case '&': {
                    switch (c2) {
                        case '&': {
                            type = CUL_LEXICAL_AND;
                            length = 2;
                            _cur++;
                            switch (c3) {
                                case '=': {
                                    type = CUL_LEXICAL_AND_EQUAL;
                                    length = 3;
                                    _cur++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case '|': {
                    switch (c2) {
                        case '|': {
                            type = CUL_LEXICAL_OR;
                            length = 2;
                            _cur++;
                            switch (c3) {
                                case '=': {
                                    type = CUL_LEXICAL_OR_EQUAL;
                                    length = 3;
                                    _cur++;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case '^': {
                    switch (c2) {
                        case '=': {
                            type = CUL_LEXICAL_XOR_EQUAL;
                            length = 2;
                            _cur++;
                            break;
                        }
                    }
                    break;
                }
            }
            token = new CULToken(word, length, type);
        }
        if (token) {
            _tokens.push_back(token);
        }
    }
}

CULLexical::~CULLexical() {
    for (CULToken *token : _tokens) {
        delete token;
    }
}

void CULLexical::next() {
    if (_index >= _tokens.size()) return;
    _index++;
}

unsigned char CULLexical::look() {
    if (_index >= _tokens.size()) return -1;
    return _tokens[_index]->_type;
}

bool CULLexical::look(unsigned char code) {
    if (_index >= _tokens.size()) return false;
    return _tokens[_index]->_type == code;
}

bool CULLexical::look(unsigned char code, unsigned char next) {
    if (_index + next > _tokens.size()) return false;
    return _tokens[_index + next]->_type == code;
}

CULToken* CULLexical::token() {
    if (_index >= _tokens.size()) return &_eof;
    return _tokens[_index];
}

CULToken* CULLexical::token(long next) {
    if (_index + next >= _tokens.size()) return &_eof;
    return _tokens[_index + next];
}

int CULLexical::utf() {
    int c = *_cur;
    if (c <= 0x7F) {
        return c;
    } else if ((c >> 5) == 0x6) {
        int i2 = *(++_cur);
        if (i2 < 0) return -1;
        return (((c & 0x1F) << 6) + (i2 & 0x3F));
    } else {
        int i2 = *(++_cur);
        if (i2 < 0) return -1;
        int i3 = *(++_cur);
        if (i3 < 0) return -1;
        return (((c & 0xF) << 12) + ((i2 & 0x3F) << 6) + (i3 & 0x3F));
    }
}