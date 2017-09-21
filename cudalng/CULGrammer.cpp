//
//  CULGrammer.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULGrammer::CULGrammer(CULLexical *lexical) {
    _lexical = lexical;
}

CULGrammer::~CULGrammer() {
    
}

CULUnit* CULGrammer::parse() throw (CULException*) {
    CULUnit *unit = new CULUnit();
    try {
        while (!_lexical->token()->eof()) {
            CULMethod *node = parseMethod();
            unit->nodes.push_back(node);
        }
        return unit;
    } catch (...) {
        delete unit;
        throw new CULException();
    }
}

CULNode* CULGrammer::parseExternalDeclaration() {
    try {
        return parseCommand();
    } catch (...) {
        throw new CULParseException();
    }
}

CULMethod* CULGrammer::parseMethod() {
    CULMethod *method;
    try {
        method = new CULMethod();
        for (;;) {
            bool quit = false;
            switch (_lexical->look()) {
                case CUL_LEXICAL_AUTO: { method->isAuto = true; _lexical->next(); break; }
                case CUL_LEXICAL_REGISTER: { method->isRegister = true; _lexical->next(); break; }
                case CUL_LEXICAL_STATIC: { method->isStatic = true; _lexical->next(); break; }
                case CUL_LEXICAL_EXTERN: { method->isExtern = true; _lexical->next(); break; }
                case CUL_LEXICAL_TYPEDEF: { method->isTypedef = true; _lexical->next(); break; }
                case CUL_LEXICAL_CONST: { method->isConst = true; _lexical->next(); break; }
                case CUL_LEXICAL_VOLATILE: { method->isVolatile = true; _lexical->next(); break; }
                case CUL_LEXICAL_DEVICE: { method->isDevice = true; _lexical->next(); break; }
                case CUL_LEXICAL_GLOBAL: { method->isGlobal = true; _lexical->next(); break; }
                case CUL_LEXICAL_KERNEL: { method->isKernel = true; _lexical->next(); break; }
                default: quit = true; break;
            }
            if (quit) break;
        }
        method->type = parseType();
        if (!_lexical->look(CUL_LEXICAL_ID)) throw "expected id";
        method->name = _lexical->token();
        _lexical->next();
        if (!_lexical->look('(')) throw "expected (";
        _lexical->next();
        if (!_lexical->look(')')) {
            method->params.push_back(parseParam());
            while (_lexical->look(',')) {
                _lexical->next();
                method->params.push_back(parseParam());
            }
        }
        _lexical->next();
        method->command = parseCommand();
        return method;
    } catch (...) {
        delete method;
        throw new CULParseException();
    }
}

CULParam* CULGrammer::parseParam() {
    CULParam *param = new CULParam();
    try {
        param->type = parseType();
        if (!_lexical->look(CUL_LEXICAL_ID)) throw "expected id";
        param->name = _lexical->token();
        _lexical->next();
        return param;
    } catch (...) {
        delete param;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseCommand() {
    switch (_lexical->look()) {
        case '{': return parseBlockCommand();
        case CUL_LEXICAL_IF: return parseIfCommand();
        case CUL_LEXICAL_FOR: return parseForCommand();
        case CUL_LEXICAL_WHILE: return parseWhileCommand();
        case CUL_LEXICAL_DO: return parseRepeatCommand();
        case CUL_LEXICAL_SWITCH: return parseSwitchCommand();
        case CUL_LEXICAL_BREAK: return parseBreakCommand();
        case CUL_LEXICAL_CONTINUE: return parseContinueCommand();
    }
    if (isDeclareCommand(0)) {
        CULDeclareCommand *cmd = parseDeclareCommand();
        if (!_lexical->look(';')) {
            delete cmd;
            throw new CULParseException();
        }
        _lexical->next();
        return cmd;
    }
    return parseExpressionCommand();
}

CULCommand* CULGrammer::parseBlockCommand() {
    CULBlockCommand *cmd = new CULBlockCommand();
    try {
        if (_lexical->look('{')) {
            _lexical->next();
            while (!_lexical->look('}')) {
                CULCommand *node = parseCommand();
                cmd->cmds.push_back(node);
                node->parent = cmd;
            }
            _lexical->next();
        } else {
            CULCommand *node = parseCommand();
            cmd->cmds.push_back(node);
            node->parent = cmd;
        }
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseIfCommand() {
    CULIfCommand *cmd = new CULIfCommand();
    try {
        _lexical->next();
        cmd->value = parseValue();
        cmd->cmd = parseBlockCommand();
        cmd->cmd->parent = cmd;
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseForCommand() {
    CULForCommand *cmd = new CULForCommand();
    try {
        _lexical->next();
        _lexical->next();
        if (!_lexical->look(';')) {
            cmd->declare = parseDeclareCommand();
            cmd->declare->parent = cmd;
        }
        _lexical->next();
        if (!_lexical->look(';')) {
            cmd->condition = parseValue();
        }
        _lexical->next();
        if (!_lexical->look(')')) {
            cmd->increment = parseValue();
        }
        _lexical->next();
        cmd->cmd = parseCommand();
        cmd->cmd->parent = cmd;
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseWhileCommand() {
    CULWhileCommand *cmd = new CULWhileCommand();
    try {
        _lexical->next();
        cmd->value = parseValue();
        cmd->cmd = parseBlockCommand();
        cmd->cmd->parent = cmd;
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseRepeatCommand() {
    CULRepeatCommand *cmd = new CULRepeatCommand();
    try {
        cmd->cmd = parseBlockCommand();
        cmd->cmd->parent = cmd;
        _lexical->next();
        cmd->value = parseValue();
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseSwitchCommand() {
    CULBlockCommand *cmd = new CULBlockCommand();
    try {
        _lexical->next();
        while (!_lexical->look('}')) {
            cmd->cmds.push_back(parseCommand());
        }
        _lexical->next();
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseBreakCommand() {
    CULBreakCommand *cmd = new CULBreakCommand();
    try {
        _lexical->next();
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseContinueCommand() {
    CULContinueCommand *cmd = new CULContinueCommand();
    try {
        _lexical->next();
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULDeclareCommand* CULGrammer::parseDeclareCommand() {
    CULDeclareCommand *cmd = new CULDeclareCommand();
    try {
        cmd->type = parseType();
        if (!_lexical->look(CUL_LEXICAL_ID)) {
            throw "expected id";
        }
        for (;;) {
            cmd->argNames.push_back(_lexical->token());
            _lexical->next();
            while (_lexical->look('[')) {
                _lexical->next();
                cmd->argNames.push_back(_lexical->token());
                _lexical->next();
                if (!_lexical->look(']')) {
                    throw "expected ]";
                }
            }
            if (_lexical->look('=')) {
                _lexical->next();
                CULValue *value = parseValue();
                cmd->argValues.push_back(value);
            } else {
                cmd->argValues.push_back(new CULNullValue());
            }
            if (!_lexical->look(',')) {
                break;
            }
            _lexical->next();
        };
        return cmd;
    } catch (...) {
        delete cmd;
        throw new CULParseException();
    }
}

CULCommand* CULGrammer::parseExpressionCommand() {
    CULValue *value = parseValue();
    // TODO memory
    CULValueCommand *cmd = new CULValueCommand(value);
    if (!_lexical->look(';')) {
        delete cmd;
        throw "expected ;";
    }
    _lexical->next();
    return cmd;
}

CULValue* CULGrammer::parseValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseAssignmentValue();
        if (_lexical->look(',')) {
            _lexical->next();
            right = parseValue();
            CULMultiValue *multiValue = new CULMultiValue();
            multiValue->left = left;
            multiValue->right = right;
            return multiValue;
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseAssignmentValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseConditionalValue();
        if (_lexical->look('=') && left->assignable()) {
            _lexical->next();
            right = parseValue();
            CULAssignmentValue *assignmentValue = new CULAssignmentValue();
            assignmentValue->left = left;
            assignmentValue->right = right;
            return assignmentValue;
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseConditionalValue() {
    CULValue *left = 0, *center = 0, *right = 0;
    try {
        left = parseLogicalOrValue();
        if (_lexical->look('?')) {
            _lexical->next();
            center = parseValue();
            if (!_lexical->look(':')) throw "expected :";
            _lexical->next();
            right = parseValue();
            CULConditionalValue *conditionalValue = new CULConditionalValue();
            conditionalValue->left = left;
            conditionalValue->center = center;
            conditionalValue->right = right;
            return conditionalValue;
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (center) delete center;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseLogicalOrValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseLogicalAndValue();
        if (_lexical->look(CUL_LEXICAL_OR)) {
            _lexical->next();
            right = parseValue();
            CULLogicalOrValue *logicOrValue = new CULLogicalOrValue();
            logicOrValue->left = left;
            logicOrValue->right = right;
            return logicOrValue;
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseLogicalAndValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseInclusiveOrValue();
        if (_lexical->look(CUL_LEXICAL_AND)) {
            _lexical->next();
            right = parseValue();
            CULLogicalAndValue *logicAndValue = new CULLogicalAndValue();
            logicAndValue->left = left;
            logicAndValue->right = right;
            return logicAndValue;
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseInclusiveOrValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseExclusiveOrValue();
        if (_lexical->look('|')) {
            _lexical->next();
            right = parseValue();
            CULInclusiveOrValue *inclusiveOrValue = new CULInclusiveOrValue();
            inclusiveOrValue->left = left;
            inclusiveOrValue->right = right;
            return inclusiveOrValue;
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseExclusiveOrValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseAndValue();
        if (_lexical->look('^')) {
            _lexical->next();
            right = parseValue();
            CULExclusiveOrValue *exclusiveOrValue = new CULExclusiveOrValue();
            exclusiveOrValue->left = left;
            exclusiveOrValue->right = right;
            return exclusiveOrValue;
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseAndValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseEquilityValue();
        if (_lexical->look('&')) {
            _lexical->next();
            right = parseValue();
            CULInclusiveAndValue *andValue = new CULInclusiveAndValue();
            andValue->left = left;
            andValue->right = right;
            return andValue;
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseEquilityValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseRelationalValue();
        switch (_lexical->look()) {
            case CUL_LEXICAL_EQUAL: {
                _lexical->next();
                right = parseValue();
                CULEqualValue *equalValue = new CULEqualValue();
                equalValue->left = left;
                equalValue->right = right;
                return equalValue;
            }
            case CUL_LEXICAL_NOTEQUAL: {
                _lexical->next();
                right = parseValue();
                CULNotEqualValue *notEqualValue = new CULNotEqualValue();
                notEqualValue->left = left;
                notEqualValue->right = right;
                return notEqualValue;
            }
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseRelationalValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseShiftValue();
        switch (_lexical->look()) {
            case '<': {
                _lexical->next();
                right = parseValue();
                CULLowerValue *lowerValue = new CULLowerValue();
                lowerValue->left = left;
                lowerValue->right = right;
                return lowerValue;
            }
            case '>': {
                _lexical->next();
                right = parseValue();
                CULGreaterValue *greaterValue = new CULGreaterValue();
                greaterValue->left = left;
                greaterValue->right = right;
                return greaterValue;
            }
            case CUL_LEXICAL_GREATER_EQUAL: {
                _lexical->next();
                right = parseValue();
                CULGreaterEqualValue *greaterEqualValue = new CULGreaterEqualValue();
                greaterEqualValue->left = left;
                greaterEqualValue->right = right;
                return greaterEqualValue;
            }
            case CUL_LEXICAL_LOWER_EQUAL: {
                _lexical->next();
                right = parseValue();
                CULLowerEqualValue *lowerEqualValue = new CULLowerEqualValue();
                lowerEqualValue->left = left;
                lowerEqualValue->right = right;
                return lowerEqualValue;
            }
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseShiftValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseAdditiveValue();
        switch (_lexical->look()) {
            case CUL_LEXICAL_SHIFT_LEFT: {
                _lexical->next();
                right = parseValue();
                CULShiftLeftValue *shiftLeftValue = new CULShiftLeftValue();
                shiftLeftValue->left = left;
                shiftLeftValue->right = right;
                return shiftLeftValue;
            }
            case CUL_LEXICAL_SHIFT_RIGHT: {
                _lexical->next();
                right = parseValue();
                CULShiftRightValue *shiftRightValue = new CULShiftRightValue();
                shiftRightValue->left = left;
                shiftRightValue->right = right;
                return shiftRightValue;
            }
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseAdditiveValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseMultValue();
        switch (_lexical->look()) {
            case '+': {
                _lexical->next();
                right = parseValue();
                CULSumValue *sumValue = new CULSumValue();
                sumValue->left = left;
                sumValue->right = right;
                return sumValue;
            }
            case '-': {
                _lexical->next();
                right = parseValue();
                CULSubValue *subValue = new CULSubValue();
                subValue->left = left;
                subValue->right = right;
                return subValue;
            }
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseMultValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parseCastValue();
        switch (_lexical->look()) {
            case '*': {
                _lexical->next();
                right = parseValue();
                CULMulValue *mulValue = new CULMulValue();
                mulValue->left = left;
                mulValue->right = right;
                return mulValue;
            }
            case '/': {
                _lexical->next();
                right = parseValue();
                CULDivValue *divValue = new CULDivValue();
                divValue->left = left;
                divValue->right = right;
                return divValue;
            }
            case '%': {
                _lexical->next();
                right = parseValue();
                CULModValue *modValue = new CULModValue();
                modValue->left = left;
                modValue->right = right;
                return modValue;
            }
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseCastValue() {
    CULValue *left = 0;
    CULType *type = 0;
    long next;
    try {
        if (_lexical->look('(') && (next = isType(1)) && isValue(next+1)) {
            _lexical->next();
            type = parseType();
            if (!_lexical->look(')')) throw "expected )";
            _lexical->next();
            left = parseValue();
            CULCastValue *castValue = new CULCastValue();
            castValue->left = left;
            castValue->type = type;
            return castValue;
        }
        return parseUnaryValue();
    } catch (...) {
        if (left) delete left;
        if (type) delete type;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parseUnaryValue() {
    CULValue *left = 0;
    try {
        switch (_lexical->look()) {
            case CUL_LEXICAL_SUMSUM: {
                _lexical->next();
                left = parseValue();
                CULPreIncValue *preIncValue = new CULPreIncValue();
                preIncValue->left = left;
                return preIncValue;
            }
            case CUL_LEXICAL_SUBSUB: {
                _lexical->next();
                left = parseValue();
                CULPreDecValue *preDecValue = new CULPreDecValue();
                preDecValue->left = left;
                return preDecValue;
            }
            case '&': {
                _lexical->next();
                left = parseValue();
                CULAddressValue *addressValue = new CULAddressValue();
                addressValue->left = left;
                return addressValue;
            }
            case '*': {
                _lexical->next();
                left = parseValue();
                CULAccessValue *accessValue = new CULAccessValue();
                accessValue->left = left;
                return accessValue;
            }
            case '+': {
                _lexical->next();
                return parseCastValue();
            }
            case '-': {
                _lexical->next();
                left = parseValue();
                CULNegativeValue *negativeValue = new CULNegativeValue();
                negativeValue->left = left;
                return negativeValue;
            }
            case '~': {
                _lexical->next();
                left = parseValue();
                CULInvertValue *invertValue = new CULInvertValue();
                invertValue->left = left;
                return invertValue;
            }
            case '!': {
                _lexical->next();
                left = parseValue();
                CULNotValue *notValue = new CULNotValue();
                notValue->left = left;
                return notValue;
            }
            case CUL_LEXICAL_SIZEOF: {
                _lexical->next();
                if (_lexical->look('(')) {
                    throw "expected (";
                }
                CULType *type = parseType();
                if (!_lexical->look(')')) {
                    delete type;
                    throw "expected )";
                }
                CULSizeofValue *sizeofValue = new CULSizeofValue();
                sizeofValue->type = type;
                return sizeofValue;
            }
        }
        return parsePostfixValue();
    } catch (...) {
        if (left) delete left;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parsePostfixValue() {
    CULValue *left = 0, *right = 0;
    try {
        left = parsePrimitiveValue();
        switch (_lexical->look()) {
            case '[': {
                _lexical->next();
                right = parseValue();
                if (!_lexical->look(']')) {
                    throw new CULParseException();
                }
                _lexical->next();
                CULArrayValue *arrayValue = new CULArrayValue();
                arrayValue->left = left;
                arrayValue->right = right;
                return arrayValue;
            }
            case '(': {
                _lexical->next();
                if (_lexical->look(')')) {
                    _lexical->next();
                } else {
                    parseArgumentValueList();
                }
                return left;
            }
            case '.': {
                _lexical->next();
                right = parseValue();
                CULGetValue *getValue = new CULGetValue();
                getValue->left = left;
                getValue->right = right;
                return getValue;
            }
            case CUL_LEXICAL_POINTER: {
                _lexical->next();
                right = parseValue();
                CULGetValue *getValue = new CULGetValue();
                getValue->left = left;
                getValue->right = right;
                return getValue;
            }
            case CUL_LEXICAL_SUMSUM: {
                _lexical->next();
                CULPosIncValue *posIncValue = new CULPosIncValue();
                posIncValue->left = left;
                return posIncValue;
            }
            case CUL_LEXICAL_SUBSUB: {
                _lexical->next();
                CULPosDecValue *posDecValue = new CULPosDecValue();
                posDecValue->left = left;
                return posDecValue;
            }
                
        }
        return left;
    } catch (...) {
        if (left) delete left;
        if (right) delete right;
        throw new CULParseException();
    }
}

CULValue* CULGrammer::parsePrimitiveValue() {
    switch (_lexical->look()) {
        case CUL_LEXICAL_ID: {
            CULIdentifyValue *idNode = new CULIdentifyValue();
            idNode->token = *_lexical->token();
            _lexical->next();
            return idNode;
        }
        case CUL_LEXICAL_NUMBER: {
            CULDoubleValue *doubleNode = new CULDoubleValue();
            doubleNode->token = *_lexical->token();
            _lexical->next();
            return doubleNode;
        }
        case CUL_LEXICAL_CHAR: {
            CULCharValue *charNode = new CULCharValue();
            charNode->token = *_lexical->token();
            _lexical->next();
            return charNode;
        }
        case CUL_LEXICAL_STRING: {
            CULStringValue *charNode = new CULStringValue();
            charNode->token = *_lexical->token();
            _lexical->next();
            return charNode;
        }
        case CUL_LEXICAL_TRUE:
        case CUL_LEXICAL_FALSE: {
            CULBooleanValue *charNode = new CULBooleanValue();
            charNode->token = *_lexical->token();
            _lexical->next();
            return charNode;
        }
        case '(': {
            _lexical->next();
            CULValue *left = parseValue();
            if (!_lexical->look(')')) {
                delete left;
                throw "expected )";
            }
            _lexical->next();
            return left;
        }
        default: {
            throw "expected value";
        }
    }
}

CULValue* CULGrammer::parseArgumentValueList() {
    return parseLogicalAndValue();
}

CULValue* CULGrammer::parseConst() {
    throw new CULParseException();
}

CULType* CULGrammer::parseType() {
    CULType* type = new CULType();
    switch (_lexical->look()) {
        case CUL_LEXICAL_SHARED: { type->isShared = true; _lexical->next(); break; }
    }
    switch (_lexical->look()) {
        case CUL_LEXICAL_ID: {
            type->size = sizeof(void*);
            _lexical->next();
            break;
        }
        case CUL_LEXICAL_INT: {
            type->size = sizeof(int);
            _lexical->next();
            break;
        }
        case CUL_LEXICAL_LONG: {
            type->size = sizeof(long);
            _lexical->next();
            break;
        }
        case CUL_LEXICAL_FLOAT: {
            type->size = sizeof(float);
            _lexical->next();
            break;
        }
        case CUL_LEXICAL_DOUBLE: {
            type->size = sizeof(double);
            _lexical->next();
            break;
        }
        case CUL_LEXICAL_CHAR: {
            type->size = sizeof(char);
            _lexical->next();
            break;
        }
        case CUL_LEXICAL_VOID: {
            type->size = sizeof(void*);
            _lexical->next();
            break;
        }
        default: throw new CULParseException();
    }
    while (_lexical->look('*')) {
        _lexical->next();
    }
    return type;
}

long CULGrammer::isType(long next) {
    long init = next;
    switch (_lexical->token(next)->_type) {
        case CUL_LEXICAL_SHARED:
            next++;
            break;
    }
    switch (_lexical->token(next)->_type) {
        case CUL_LEXICAL_ID:
        case CUL_LEXICAL_INT:
        case CUL_LEXICAL_LONG:
        case CUL_LEXICAL_FLOAT:
        case CUL_LEXICAL_DOUBLE:
        case CUL_LEXICAL_CHAR:
            next++;
            break;
        default: return 0;
    }
    while (_lexical->look('*', next)) {
        next++;
    }
    while (_lexical->look('[', next)) {
        next++;
        if (!_lexical->look(CUL_LEXICAL_NUMBER, next)) return 0;
        next++;
        if (!_lexical->look(']', next)) return 0;
        next++;
    }
    return next - init;
}

long CULGrammer::isValue(long next) {
    switch (_lexical->token(next)->_type) {
        case '(':
        case '&':
        case '*':
        case '+':
        case '-':
        case '~':
        case '!':
        case CUL_LEXICAL_SUMSUM:
        case CUL_LEXICAL_SUBSUB:
        case CUL_LEXICAL_SIZEOF:
        case CUL_LEXICAL_ID:
        case CUL_LEXICAL_NUMBER:
        case CUL_LEXICAL_CHAR:
        case CUL_LEXICAL_STRING:
        case CUL_LEXICAL_TRUE:
        case CUL_LEXICAL_FALSE:
            return 1;
    }
    return 0;
}

long CULGrammer::isDeclareCommand(long next) {
    long n = isType(next);
    if (n <= 0) return 0;
    next += n;
    if (!_lexical->look(CUL_LEXICAL_ID, next)) return 0;
    next++;
    if (_lexical->look(',', next) || _lexical->look(';', next) || _lexical->look('=', next)) return next + 1;
    return 0;
}
