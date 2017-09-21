//
//  main.cpp
//  cudalng
//
//  Created by Bernardo Breder on 20/05/15.
//  Copyright (c) 2015 Cuda. All rights reserved.
//

#include <iostream>
#include "CUL.h"
#include "CULTest.h"

//Quantidade de memória utilizada (true)
//Relação entre o uso de memória compartilha e global (true)
//Quantidade de Thread e Blocos utilizados (true)
//Quantidade de memória transferida da CPU para GPU (true)
//Quantidade de memória retornada da GPU para CPU (true)
//Número de operações em um exemplo aleatório (false)
//Número de operações aproximada através da analise sintática (true)
//Tempo médio de execução em função do histórico (false)
//Quantidade de uso de sincronismo (true)
//Número de operações com inteiro (false)
//Número de operações com float (false)
//Número de operações com double (false)
//Número de operações atômicos (false)

unsigned char* cgl_io_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return 0;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned char* data = (unsigned char*) malloc(size + 1);
    if (!data) {
        fclose(file);
        return 0;
    }
    size_t readed = fread(data, 1, size, file);
    if (readed != size) {
        free(data);
        fclose(file);
        return 0;
    }
    data[size] = 0;
    fclose(file);
    return data;
}

int main(int argc, const char * argv[]) {
//    CULTest();
    
    unsigned char *content = cgl_io_file("/Users/bernardobreder/breder/Dropbox/cudalng/cudalng/test/test_add_loop_gpu.cu");
    if (!content) {
        printf("File not found\n");
        exit(1);
    }
    
    try {
        CULLexical lexical((const char*) content);
        CULGrammer grammer(&lexical);
        CULUnit* unit = grammer.parse();
        CULContext* context = new CULContext();
        unit->syntaxHead(context);
        unit->syntaxBody(context);
        unit->semantic(context);
        printf("Complexity: O(n^%d)\n", context->maxComplexity);
        printf("Shared/Global Memory: %0.2f\n", (double) context->sharedMemory / context->globalMemory);
        printf("Uso de Shared Memory: %d\n", context->sharedMemory);
        printf("Uso de Global Memory: %d\n", context->globalMemory);
        printf("Uso de __syncthreads(): %d\n", context->numberSync);
        printf("Number of Declare: %d\n", context->numberOfDeclare);
        printf("Number of If: %d\n", context->numberOfIf);
        printf("Number of While: %d\n", context->numberOfWhile);
        printf("Number of Repeat: %d\n", context->numberOfRepeat);
        printf("Number of For: %d\n", context->numberOfFor);
        printf("Complexity of Operatator Sum: ");
        for (int n = 0 ; n <= context->maxComplexity ; n++) {
            printf("%d ", context->sumCompexity[n]);
        }
        printf("\n");
        delete context;
        delete unit;
    } catch (CULException* e) {
        printf("CULException");
    }
    
    free(content);
    return 0;
}
