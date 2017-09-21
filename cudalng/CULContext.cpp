//
//  CULContext.cpp
//  cudalng
//
//  Created by Bernardo Breder on 9/7/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include "CUL.h"

CULContext::CULContext() {
    for (int n = 0 ; n < CULCONTEXT_MAX_COMPLEXITY ; n++) {
        sumCompexity[n] = 0;
    }
}