
#define N 10

int main( void ) {
    int a[N], b[N], c[N];
    int *dev_a, *dev_b, *dev_c;

    // allocate the memory on the GPU
    cul_cudaMalloc( (void**)&dev_a, N * sizeof(int) );
    cul_cudaMalloc( (void**)&dev_b, N * sizeof(int) );
    cul_cudaMalloc( (void**)&dev_c, N * sizeof(int) );

    // fill the arrays 'a' and 'b' on the CPU
    for (int i=0; i<N; i++) {
        a[i] = -i;
        b[i] = i * i;
    }

    // copy the arrays 'a' and 'b' to the GPU
    cul_cudaMemory( dev_a, a, N * sizeof(int), cudaMemcpyHostToDevice );
    cul_cudaMemory( dev_b, b, N * sizeof(int), cudaMemcpyHostToDevice );

    cul_recordExecution(add, 1, N, 1);
	add<<<N,1>>>( dev_a, dev_b, dev_c );

    // copy the array 'c' back from the GPU to the CPU
    cul_cudaMemory( c, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost );

    // display the results
    for (int i=0; i<N; i++) {
        printf( "%d + %d = %d\n", a[i], b[i], c[i] );
    }

    // free the memory allocated on the GPU
    cudaFree( dev_a );
    cudaFree( dev_b );
    cudaFree( dev_c );

    return 0;
}
