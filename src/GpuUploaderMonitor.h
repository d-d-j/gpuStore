/*
 * GpuUploaderMonitor.h
 *
 *  Created on: Aug 31, 2013
 *      Author: Karol Dzitkowski
 */

#include "GpuUploaderCore.h"
#include "BTreeMonitor.h"

#ifndef GPUUPLOADERMONITOR_H_
#define GPUUPLOADERMONITOR_H_

namespace ddj {
namespace store {

	class GpuUploaderMonitor
	{
		private:
			boost::thread* _threadUploader;

			boost::condition_variable _condUploader;
			boost::condition_variable _condSynchronization;

			boost::mutex _mutexSynchronization;
			boost::mutex _mutexUploader;

			BTreeMonitor* _bTreeInserter;


		public:
			GpuUploaderMonitor(BTreeMonitor* bTreeInserter);
			virtual ~GpuUploaderMonitor();

		private:

	};

} /* namespace store */
} /* namespace ddj */
#endif /* GPUUPLOADERMONITOR_H_ */
