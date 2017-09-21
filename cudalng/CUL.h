//
//  CUL.h
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#ifndef __cudalng__CUL__
#define __cudalng__CUL__

#include <stdio.h>
#include <vector>


#define CUL_LEXICAL_ID 1
#define CUL_LEXICAL_CHARACTER 2
#define CUL_LEXICAL_NUMBER 3
#define CUL_LEXICAL_STRING 4

#define CUL_LEXICAL_OR 150
#define CUL_LEXICAL_AND 151
#define CUL_LEXICAL_EQUAL 152
#define CUL_LEXICAL_NOTEQUAL 153
#define CUL_LEXICAL_GREATER_EQUAL 154
#define CUL_LEXICAL_LOWER_EQUAL 155
#define CUL_LEXICAL_SHIFT_LEFT 156
#define CUL_LEXICAL_SHIFT_RIGHT 157
#define CUL_LEXICAL_SUMSUM 158
#define CUL_LEXICAL_SUBSUB 159
#define CUL_LEXICAL_POINTER 160
#define CUL_LEXICAL_MUL_EQUAL 161
#define CUL_LEXICAL_DIV_EQUAL 162
#define CUL_LEXICAL_SUM_EQUAL 163
#define CUL_LEXICAL_SUB_EQUAL 164
#define CUL_LEXICAL_SHIFT_LEFT_EQUAL 165
#define CUL_LEXICAL_SHIFT_RIGHT_EQUAL 166
#define CUL_LEXICAL_AND_EQUAL 167
#define CUL_LEXICAL_XOR_EQUAL 168
#define CUL_LEXICAL_OR_EQUAL 169
#define CUL_LEXICAL_DOT3 171

#define CUL_LEXICAL_IF 130
#define CUL_LEXICAL_ELSE 219
#define CUL_LEXICAL_GOTO 220
#define CUL_LEXICAL_FOR 131
#define CUL_LEXICAL_WHILE 132
#define CUL_LEXICAL_DO 133
#define CUL_LEXICAL_SWITCH 134
#define CUL_LEXICAL_BREAK 135
#define CUL_LEXICAL_CONTINUE 136
#define CUL_LEXICAL_TRUE 137
#define CUL_LEXICAL_FALSE 138
#define CUL_LEXICAL_RETURN 139
#define CUL_LEXICAL_SIZEOF 170
#define CUL_LEXICAL_AUTO 200
#define CUL_LEXICAL_REGISTER 201
#define CUL_LEXICAL_STATIC 202
#define CUL_LEXICAL_EXTERN 203
#define CUL_LEXICAL_TYPEDEF 204
#define CUL_LEXICAL_VOID 205
#define CUL_LEXICAL_CHAR 206
#define CUL_LEXICAL_SHORT 207
#define CUL_LEXICAL_INT 208
#define CUL_LEXICAL_LONG 209
#define CUL_LEXICAL_FLOAT 210
#define CUL_LEXICAL_DOUBLE 211
#define CUL_LEXICAL_SIGNED 212
#define CUL_LEXICAL_UNSIGNED 213
#define CUL_LEXICAL_CONST 214
#define CUL_LEXICAL_VOLATILE 215
#define CUL_LEXICAL_UNION 216
#define CUL_LEXICAL_STRUCT 217
#define CUL_LEXICAL_ENUM 218
#define CUL_LEXICAL_KERNEL 221
#define CUL_LEXICAL_DEVICE 222
#define CUL_LEXICAL_GLOBAL 223
#define CUL_LEXICAL_SHARED 224

#define CUL_LEXICAL_LOOK_MAX 0xFF

#define cul_cudaMalloc(dest, size) \
CUL_cudaMalloc(dest, size); \
cudaMalloc(dest, size);

#define cul_cudaFree(dest) \
cudaFree(dest);

#define cul_cudaMemory(dest, src, size, direction) \
CUL_cudaMemory(dest, src, size, direction); \
cudaMemcpy(dest, src, size, direction);

#define cul_execute(func, grids, blocks, threads) \
CUL_Execute(func, grids, blocks, threads);

#define CULCONTEXT_MAX_COMPLEXITY 0xFF

class CULContext {
    
public:
    
    int maxComplexity = 0;

    int curComplexity = 0;

    int sharedMemory = 0;

    int globalMemory = 0;
    
    int numberSync = 0;
    
    int numberOfIf = 0;
    
    int numberOfWhile = 0;
    
    int numberOfRepeat = 0;
    
    int numberOfFor = 0;
    
    int numberOfDeclare = 0;
    
    int sumCompexity[CULCONTEXT_MAX_COMPLEXITY];
    
    CULContext();

};

class CULException {
    
};

class CULParseException: CULException {
    
};


class CULToken {
    
private:
    
protected:
    
public:
    
    const char* _word;
    
    long _len;
    
    unsigned char _type;
    
    CULToken(const char* word, long length, unsigned char type);
    
    CULToken();
    
    unsigned char type();
    
    const char* word();
    
    long length();
    
    bool eof();
    
};

class CULNode {
    
private:
    
protected:
    
public:
    
    CULNode();
    
    virtual ~CULNode();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULType : public CULNode {
    
public:
    
    bool isShared = false;
    
    int size = 0;
    
};

class CULValue : public CULNode {
    
private:
    
protected:
    
public:
    
    CULType *type;
    
    virtual bool assignable();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULCommand : public CULNode {
    
private:
    
protected:
    
public:
    
    CULCommand *parent;
    
    CULCommand();
    
    virtual ~CULCommand();
    
    virtual bool isLoop();
    
    void refreshComplexity(CULContext *context);
    
};

class CULBlockCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    std::vector<CULCommand*> cmds;
    
    CULBlockCommand();
    
    virtual ~CULBlockCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULDeclareCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULType *type;
    
    std::vector<CULValue*> argValues;
    
    std::vector<CULToken*> argNames;
    
    CULDeclareCommand();
    
    virtual ~CULDeclareCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULIfCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULValue *value;
    
    CULCommand *cmd;
    
    CULIfCommand();
    
    virtual ~CULIfCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULWhileCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULValue *value;
    
    CULCommand *cmd;
    
    CULWhileCommand();
    
    virtual ~CULWhileCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
    virtual bool isLoop();
    
};


class CULRepeatCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULValue *value;
    
    CULCommand *cmd;
    
    CULRepeatCommand();
    
    virtual ~CULRepeatCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
    virtual bool isLoop();
    
};

class CULForCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULCommand *declare;
    
    CULValue *condition;
    
    CULValue *increment;
    
    CULCommand *cmd;
    
    CULForCommand();
    
    virtual ~CULForCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
    virtual bool isLoop();
    
};

class CULSwitchCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULSwitchCommand();
    
    virtual ~CULSwitchCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};


class CULContinueCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULContinueCommand();
    
    virtual ~CULContinueCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};


class CULBreakCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULBreakCommand();
    
    virtual ~CULBreakCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};


class CULReturnCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULReturnCommand();
    
    virtual ~CULReturnCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};


class CULGotoCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULGotoCommand();
    
    virtual ~CULGotoCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULValueCommand : public CULCommand {
    
private:
    
protected:
    
public:
    
    CULValue *_value;
    
    CULValueCommand(CULValue *value);
    
    ~CULValueCommand();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULGetValue : public CULValue {
    
private:
    
protected:
    
public:
    
    CULValue *left;
    
    CULValue *right;
    
    virtual ~CULGetValue();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULArrayValue : public CULValue {
    
private:
    
protected:
    
public:
    
    CULValue *left;
    
    CULValue *right;
    
    virtual ~CULArrayValue();
    
};

class CULUnaryValue : public CULValue {
    
private:
    
protected:
    
public:
    
    CULValue *left;
    
    virtual ~CULUnaryValue();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULPointerValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULAddressValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULAccessValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULNegativeValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULInvertValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULNotValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULPreIncValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULPreDecValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULPosIncValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULPosDecValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULCastValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULSizeofValue : public CULValue {
    
private:
    
protected:
    
public:
    
    CULType *type;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULUnit : public CULNode {
    
public:
    
    std::vector<CULNode*> nodes;
    
    virtual ~CULUnit();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULParam : public CULNode {
    
public:
    
    CULToken *name;
    
    CULType *type;
    
    virtual ~CULParam();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULMethod : public CULNode {
    
public:
    
    bool isAuto;
    
    bool isRegister;
    
    bool isStatic;
    
    bool isExtern;
    
    bool isTypedef;
    
    bool isConst;
    
    bool isVolatile;
    
    bool isDevice;
    
    bool isKernel;
    
    bool isGlobal;
    
    CULToken *name;
    
    CULType *type;
    
    std::vector<CULParam*> params;
    
    CULCommand *command;
    
    virtual ~CULMethod();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULBinaryValue : public CULUnaryValue {
    
private:
    
protected:
    
public:
    
    CULValue *right;
    
    virtual ~CULBinaryValue();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULLogicalOrValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULLogicalAndValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULInclusiveOrValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULExclusiveOrValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULInclusiveAndValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULEqualValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULNotEqualValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULLowerValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULLowerEqualValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULGreaterValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULGreaterEqualValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULShiftLeftValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULShiftRightValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULSumValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULSubValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULMulValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULDivValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULModValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};


class CULAssignmentValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULMultiValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);

};

class CULTernaryValue : public CULBinaryValue {
    
private:
    
protected:
    
public:
    
    CULValue *center;
    
    virtual ~CULTernaryValue();
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULConditionalValue : public CULTernaryValue {
    
private:
    
protected:
    
public:
    
};

class CULLiteralValue : public CULValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULNullValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULCharValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    CULToken token;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULBooleanValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    CULToken token;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULStringValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    CULToken token;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULIntegerValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    CULToken token;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULLongValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    CULToken token;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULFloatValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    CULToken token;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULDoubleValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    CULToken token;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULIdentifyValue : public CULLiteralValue {
    
private:
    
protected:
    
public:
    
    CULToken token;
    
    virtual void syntaxHead(CULContext *context);
    
    virtual void syntaxBody(CULContext *context);
    
    virtual void semantic(CULContext *context);
    
};

class CULLexical {
    
private:
    
    std::vector<CULToken*> _tokens;
    
    long _index;
    
    const char* _text;
    
    char* _cur;
    
    CULToken _next;
    
    CULToken _look[0xFF];
    
    unsigned char _lookIndex;
    
    CULToken _eof;
    
protected:
    
    int utf();
    
    void read();
    
public:
    
    CULLexical(const char* text);
    
    ~CULLexical();
    
    void next();
    
    unsigned char look();
    
    bool look(unsigned char code);
    
    bool look(unsigned char code, unsigned char next);
    
    CULToken* token();
    
    CULToken* token(long index);
    
};

class CULGrammer {
    
private:
    
    CULLexical *_lexical;
    
protected:
    
    CULMethod* parseMethod();
    CULParam* parseParam();
    CULNode* parseExternalDeclaration();
    
    CULCommand* parseCommand();
    
    CULCommand* parseBlockCommand();
    CULDeclareCommand* parseDeclareCommand();
    CULCommand* parseForCommand();
    CULCommand* parseWhileCommand();
    CULCommand* parseRepeatCommand();
    CULCommand* parseSwitchCommand();
    CULCommand* parseBreakCommand();
    CULCommand* parseContinueCommand();
    CULCommand* parseIfCommand();
    CULCommand* parseExpressionCommand();
    
    CULValue* parseValue();
    CULValue* parseAssignmentValue();
    CULValue* parseConditionalValue();
    CULValue* parseLogicalOrValue();
    CULValue* parseLogicalAndValue();
    CULValue* parseInclusiveOrValue();
    CULValue* parseExclusiveOrValue();
    CULValue* parseAndValue();
    CULValue* parseEquilityValue();
    CULValue* parseRelationalValue();
    CULValue* parseShiftValue();
    CULValue* parseAdditiveValue();
    CULValue* parseMultValue();
    CULValue* parseCastValue();
    CULValue* parseUnaryValue();
    CULValue* parsePostfixValue();
    CULValue* parsePrimitiveValue();
    CULValue* parseArgumentValueList();
    CULValue* parseConst();
    
    CULType* parseType();
    
    long isType(long next);
    long isValue(long next);
    long isDeclareCommand(long next);
    
public:
    
    CULGrammer(CULLexical *lexical);
    
    ~CULGrammer();
    
    CULUnit* parse() throw (CULException*);
    
};

#endif /* defined(__cudalng__CUL__) */
