/*
 * QueryCore.h
 *
 *  Created on: 19-09-2013
 *      Author: ghashd
 */

#ifndef QUERYCORE_H_
#define QUERYCORE_H_

#include "StoreQuery.h"
#include "StoreElement.h"
#include "../Cuda/CudaController.h"
#include "../Cuda/CudaIncludes.h"
#include <gtest/gtest.h>

namespace ddj {
namespace store {

	class StoreQueryCore
	{
	private:
		CudaController* _cudaController;
	public:
		StoreQueryCore(CudaController* cudaController);
		virtual ~StoreQueryCore();

		/*
		 * Description:
		 * Method executing query with optional selected dataLocations
		 * If no dataLocation provided query is executed to all data in store
		 * Returns:
		 * 	size of data produced by query
		 * Output:
		 * 	result of query is returned to queryResult parameter
		 */
		size_t ExecuteQuery(void** queryResult, storeQuery* query, boost::container::vector<ullintPair>* dataLocationInfo = nullptr);

	private:
		/* DATA MANAGEMENT METHODS */
		size_t mapData(void** data, boost::container::vector<ullintPair>* dataLocationInfo = nullptr);
		storeElement* decompressData(void* data, size_t size);
		size_t filterData(storeElement* elements, storeQuery* query);

		/* AGGREGATION MATHODS */
		void add(storeQuery* query);




	private:
		/* TESTS */
		friend class StoreQueryCoreTest;
		FRIEND_TEST(StoreQueryCoreTest, mapData_AllData);
		FRIEND_TEST(StoreQueryCoreTest, mapData_ChooseOneTrunk);
		FRIEND_TEST(StoreQueryCoreTest, mapData_ChooseManyTrunks);
		FRIEND_TEST(StoreQueryCoreTest, filterData_EmptyFilter);
		FRIEND_TEST(StoreQueryCoreTest, filterData_NonEmptyFilter);
	};

} /* namespace store */
} /* namespace ddj */
#endif /* QUERYCORE_H_ */
