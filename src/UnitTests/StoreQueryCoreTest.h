/*
 * StoreQueryCoreTest.h
 *
 *  Created on: 17-12-2013
 *      Author: ghash
 */

#ifndef STOREQUERYCORETEST_H_
#define STOREQUERYCORETEST_H_

#define _USE_MATH_DEFINES
#define STORE_QUERY_CORE_TEST_MEM_SIZE 1024

#include "../Store/StoreQueryCore.h"
#include "../Cuda/CudaController.h"
#include "../Core/Logger.h"
#include "../Cuda/CudaIncludes.h"
#include <gtest/gtest.h>
#include <cmath>

// CUDA
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include "../Core/helper_cuda.h"
#include <thrust/version.h>

namespace ddj {
namespace store {

	class StoreQueryCoreTest : public ::testing::Test
	{
		int _devId;

	protected:
		StoreQueryCoreTest()
		{
			const char* argv = "";
			_devId = findCudaDevice(0, &argv);
			_cudaController = nullptr;
			_queryCore = nullptr;
		}

		virtual void SetUp()
		{
			_cudaController = new CudaController(3,3,_devId);
			_queryCore = new StoreQueryCore(_cudaController);
		}
		virtual void TearDown()
		{
			delete _cudaController;
			delete _queryCore;
		}

		void createSimpleCharTestData();
		void createTestDataWithStoreElements();

		StoreQueryCore* _queryCore;
		CudaController* _cudaController;
	};

} /* namespace store */
} /* namespace ddj */
#endif /* STOREQUERYCORETEST_H_ */
