/*
 *  StoreController.cpp
 *  StoreController
 *
 *  Created by Karol Dzitkowski on 27.07.2013.
 *  Copyright (c) 2013 Karol Dzitkowski. All rights reserved.
 *
 *      NAZEWNICTWO
 * 1. nazwy klas:  CamelStyle z dużej litery np. StoreController
 * 2. nazwy struktur camelStyle z małej litery np. storeElement
 * 3. nazwy pól prywatnych camelStyle z małej litery z podkreśleniem _backBuffer
 * 4. nazwy pól publicznych i zmiennych globalnych słowa rozdzielamy _ i z małych liter np. memory_available
 * 5. define z dużych liter i rozdzielamy _ np. BUFFER_SIZE
 * 6. nazwy metod publicznych z dużej litery CamelStyle np. InsertValue() oraz parametry funkcji z małych liter camelStyle np. InsertValue(int valToInsert);
 * 7. nazwy metod prywatnych z małej litery camelStyle
 * 8. nazwy funkcji "prywatnych" w plikach cpp z małej litery z _ czyli, insert_value(int val_to_insert);
 * 9. nazwy funkcji globalnych czyli w plikach .h najczęściej inline h_InsertValue() dla funkcji na CPU g_InsertValue() dla funkcji na GPU
 */

#include "StoreController.h"

namespace ddj {
namespace store {

	StoreController::StoreController()
	{
		h_LogThreadDebug("StoreController constructor started");

		this->_buffers = new __gnu_cxx::hash_map<tag_type, StoreBuffer_Pointer>();
		this->_taskBarrier = new boost::barrier(2);

		// START TASK THRAED
		this->_taskThread = new boost::thread(boost::bind(&StoreController::taskThreadFunction, this));
		this->_taskBarrier->wait();

		h_LogThreadDebug("StoreController constructor ended");
	}

	StoreController::~StoreController()
	{
		h_LogThreadDebug("StoreController destructor started");

		// STOP TASK THREAD
		this->_taskThread->interrupt();
		this->_taskThread->join();

		delete this->_buffers;
		delete this->_taskBarrier;
		delete this->_taskThread;

		h_LogThreadDebug("StoreController destructor ended");
	}

	void StoreController::CreateTask(int taskId, TaskType type, void* taskData, int dataSize)
	{
		// Add a new task to task monitor

		// Fire a function from _TaskFunctions with this taskId
		//this->_taskFunctions[taskId]();
	}

	void StoreController::taskThreadFunction()
	{
		h_LogThreadDebug("Task thread started");
		boost::unique_lock<boost::mutex> lock(this->_taskMutex);
		h_LogThreadDebug("Task thread locked his mutex");
		this->_taskBarrier->wait();
		while(1)
		{
			h_LogThreadDebug("Task thread is waiting");
			this->_taskCond.wait(lock);
			h_LogThreadDebug("Task thread is doing his job");
			// TODO: Implement taskThread real job...
		}
	}


	/*
	bool StoreController::InsertValue(storeElement* element)
	{
		if(_buffers->count(element->tag) == 0)
		{
			std::shared_ptr<StoreBuffer> p(new StoreBuffer(element->tag));
			_buffers->insert(store_hash_value_type( element->tag, p));
		}
		return (*_buffers)[element->tag]->InsertElement(element);
	}

	bool StoreController::InsertValue(int series, tag_type tag, ullint time, store_value_type value)
	{
		return this->InsertValue(new storeElement(series, tag, time, value));
	}
	*/

} /* namespace store */
} /* namespace ddj */
