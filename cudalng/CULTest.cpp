//
//  CULTest.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include <iostream>
#include <time.h>
#include "CULTest.h"

#define ASSERT_FAIL(S) { std::cout << "[" << __FILE__ << ":" << __LINE__ << "]: " << S << "\n"; return; }
#define ASSERT_NOTNULL(V) if (!(V)) { ASSERT_FAIL("Expected not null"); }
#define ASSERT_TRUE(V) if (!(V)) { ASSERT_FAIL("Expected not null"); }

static void testLexicalEmpty() {
//    CULLexical *lexical = new CULLexical("");
//    ASSERT_NOTNULL(lexical->next());
//    ASSERT_NOTNULL(lexical->look());
//    ASSERT_TRUE(lexical->look()->eof());
//    for (int n = 0 ; n < 10 ; n++) {
//        ASSERT_NOTNULL(lexical->next());
//        ASSERT_NOTNULL(lexical->look());
//        ASSERT_TRUE(lexical->look()->eof());
//    }
//    delete lexical;
}

#define CULTestExecute(F,N) { CULTest_BeforeTest(); F(); CULTest_AfterTest(N); }

static clock_t testTime;

void CULTest_BeforeTests() {
    std::cout << "Initing all Tests!\n";
}

void CULTest_AfterTests() {
    std::cout << "Finished all Tests!\n";
}

void CULTest_BeforeTest() {
    testTime = clock();
}

void CULTest_AfterTest(const char* name) {
    std::cout << "[" << name << "]" << "[" << (clock() - testTime) << "]" << " " << "Ok" << "\n";
}

void CULTest() {
    CULTest_BeforeTests();
    CULTestExecute(testLexicalEmpty, "testLexicalEmpty");
    CULTest_AfterTests();
}

