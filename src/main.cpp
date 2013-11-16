/*
 * main.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: Karol Dzitkowski
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

#include "Node.h"
#include "Store/storeElement.h"
#include "Network/Client.h"
#include <cstdint>
#include "Helpers/Logger.h"

#include "Helpers/Config.h"

using namespace ddj::store;

void InitializeLogger()
{
    log4cplus::initialize();
    LogLog::getLogLog()->setInternalDebugging(true);
	PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("ddj_logger.prop"));
}


int main(int ac, char* av[])
{
	Config::GetInstance();

	InitializeLogger();
	Logger logger = Logger::getRoot();

	LOG4CPLUS_INFO(logger, LOG4CPLUS_TEXT("Node main application started"));

	ddj::Node n;

	getchar();

	return EXIT_SUCCESS;
}


