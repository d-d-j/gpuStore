#include "CudaController.h"

namespace ddj {
namespace store {

	CudaController::CudaController(int uploadStreamsNum, int queryStreamsNum)
	{
		LOG4CPLUS_DEBUG(this->_logger, LOG4CPLUS_TEXT("Cuda controller constructor [BEGIN]"));

		this->_numUploadStreams = uploadStreamsNum;
		this->_numQueryStreams = queryStreamsNum;
		_uploadStreams = new cudaStream_t[this->_numUploadStreams];
		_queryStreams = new cudaStream_t[this->_numQueryStreams];

		for(int k=0; k<this->_numUploadStreams; k++)
			cudaStreamCreate(&(_uploadStreams[k]));

		for(int l=0; l<this->_numQueryStreams; l++)
					cudaStreamCreate(&(_queryStreams[l]));

		this->_mainMemoryOffset = 0;
		this->_mainMemoryPointer = NULL;

		this->allocateMainGpuStorage();

		LOG4CPLUS_DEBUG(this->_logger, LOG4CPLUS_TEXT("Cuda controller constructor [END]"));
	}

	CudaController::~CudaController()
	{
		LOG4CPLUS_DEBUG(this->_logger, LOG4CPLUS_TEXT("Cuda controller destructor [BEGIN]"));

		// RELEASE UPLOAD STREAMS
		for(int i = 0; i < this->_numUploadStreams; i++)
			cudaStreamDestroy(this->_uploadStreams[i]);
		delete [] this->_uploadStreams;

		// RELEASE QUERY STREAMS
		for(int i = 0; i < this->_numQueryStreams; i++)
				cudaStreamDestroy(this->_queryStreams[i]);
			delete [] this->_queryStreams;

		// RELEASE MAIN GPU STORE MEMORY
		_cudaCommons.CudaFreeMemory(this->_mainMemoryPointer);

		LOG4CPLUS_DEBUG(this->_logger, LOG4CPLUS_TEXT("Cuda controller destructor [END]"));
	}

	ullint CudaController::GetMainMemoryOffset()
	{
		boost::mutex::scoped_lock lock(_offsetMutex);
		return this->_mainMemoryOffset;
	}

	void CudaController::SetMainMemoryOffset(ullint offset)
	{
		boost::mutex::scoped_lock lock(_offsetMutex);
		LOG4CPLUS_DEBUG_FMT(this->_logger, "Setting main memory offset to: %llu", offset);
		this->_mainMemoryOffset = offset;
	}

	cudaStream_t CudaController::GetUploadStream(int num)
	{
		return this->_uploadStreams[num];
	}

	cudaStream_t CudaController::GetQueryStream(int num)
	{
		return this->_queryStreams[num];
	}

	void* CudaController::GetMainMemoryPointer()
	{
		boost::mutex::scoped_lock lock(_offsetMutex);
		return this->_mainMemoryPointer;
	}

	void* CudaController::GetMainMemoryFirstFreeAddress()
	{
		boost::mutex::scoped_lock lock(_offsetMutex);
		return (char*)this->_mainMemoryPointer+this->_mainMemoryOffset;
	}

	void CudaController::allocateMainGpuStorage()
	{
		int maxAttempts = _config->GetIntValue("GPU_MEMORY_ALLOC_ATTEMPTS");
		int memorySize = _config->GetIntValue("MAIN_STORE_SIZE");
		cudaError_t error = cudaSuccess;
		while(maxAttempts)
		{
			LOG4CPLUS_INFO(this->_logger, "Allocating " << memorySize << " of memory...");

			error = _cudaCommons.CudaAllocateArray(memorySize, &(this->_mainMemoryPointer));

			if(error != cudaSuccess)
			{
				LOG4CPLUS_ERROR(this->_logger, "CUDA ERROR - Can't allocate " << memorySize << " B of GPU memory - " << cudaGetErrorString(error));
			}
			else
			{
				LOG4CPLUS_INFO(this->_logger, "CUDA SUCCESS - allocated " << memorySize << " B of GPU memory");
				break;
			}
			maxAttempts--;
			memorySize /= 2;
		}
		if(!maxAttempts)	// if memory cannot be allocated throw an exception
		{
			LOG4CPLUS_FATAL_FMT(this->_logger, "CUDA FATAL ERROR MAIN MEMORY ALLOCATION - %s", cudaGetErrorString(error));
			throw std::runtime_error("Cannot allocate main GPU memory in storeController");
		}
	}

} /* namespace store */
} /* namespace ddj */
