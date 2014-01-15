#include "CompressionLightweight.cuh"

union converter {
	int32_t toInt, fromInt;
	float toFloat, fromFloat;
	unsigned char toBytes[4], fromBytes[4];
};

__device__
void copyBytes(unsigned char * dest, const unsigned char * source,
		const int size) {
	for (int i = 0; i < size; i++) {
		dest[i] = source[i];
	}
}

__global__
void EncodeKernel(storeElement * in_d, unsigned char * out_d) {
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int32_t low = in_d[index].time & 0xFFFFFFFF;
	int32_t high = (in_d[index].time >> 32) & 0xFFFFFFFF;
	int32_t position = 10 * index + 4;
	converter c;
	if (index == 0) {
		c.fromInt = high;
		copyBytes(out_d, c.toBytes, 4);
	}
	out_d[position] = (unsigned char) in_d[index].tag;
	position++;
	out_d[position] = (unsigned char) in_d[index].metric;
	position++;

	c.fromInt = low;
	copyBytes(out_d + position, c.toBytes, 4);

	position += 4;

	c.fromFloat = in_d[index].value;
	copyBytes(out_d + position, c.toBytes, 4);
}

__global__
void DecodeKernel(unsigned char * in_d, storeElement * out_d) {
	int index = blockIdx.x * blockDim.x + threadIdx.x;
	converter c;
	copyBytes(c.fromBytes, in_d, 4);

	int64_t high = c.toInt;
	int32_t position = 10 * index + 4;

	out_d[index].tag = in_d[position];
	position++;
	out_d[index].metric = in_d[position];
	position++;

	copyBytes(c.fromBytes, in_d + position, 4);
	out_d[index].time = ((int64_t) c.toInt & 0xFFFFFFFF) | (high << 32);

	position += 4;

	copyBytes(c.fromBytes, in_d + position, 4);
	out_d[index].value = c.toFloat;
}

size_t CompressLightweight(storeElement* elements, size_t size, unsigned char** result)
{
	int elemCount = size / sizeof(storeElement);

	//prepare compression output
	unsigned char *compressionOutput_device;	//output space for compressed data
	cudaMalloc((void**) &compressionOutput_device, COMPRESSED_DATA_SIZE(elemCount));
	cudaMemset(compressionOutput_device, 0, COMPRESSED_DATA_SIZE(elemCount));

	//compress
	int blocks = (elemCount + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
	EncodeKernel<<<blocks, THREADS_PER_BLOCK>>>(elements, compressionOutput_device);
	cudaDeviceSynchronize();

	//return result
	(*result) = compressionOutput_device;
	return COMPRESSED_DATA_SIZE(elemCount);
}

size_t DecompressLightweight(unsigned char* data, size_t size, storeElement** result)
{
	int elemCount = size / COMPRESSED_ELEMENT_SIZE;

	// prepare decompression output
	storeElement* decompressionOutput_device;	//output space for decompressed data
	cudaMalloc((void**) &decompressionOutput_device, elemCount * sizeof(storeElement));
	cudaMemset(decompressionOutput_device, 0, elemCount * sizeof(storeElement));

	//decompress
	int blocks = (elemCount + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
	unsigned char *decompessionInput_device = data;
	DecodeKernel<<<blocks, THREADS_PER_BLOCK>>>(decompessionInput_device, decompressionOutput_device);
	cudaDeviceSynchronize();

	(*result) = decompressionOutput_device;
	return elemCount * sizeof(storeElement);
}
