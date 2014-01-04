#include "CudaCompression.cuh"

void compressVar(int max_size, int bl, int *dev_data, char* dev_out)
{
	compress_var <<<max_size / (8*1000) , 100>>> (5, dev_data, dev_out);
}

void decompressVar(int max_size, int bl, int *dev_data, char* dev_out)
{
	decompress_var<<<max_size / (8*1000), 100 >>> (5, dev_data, dev_out);
}
