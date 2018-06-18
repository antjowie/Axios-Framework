#include "Axios/Instance.h"

int main()
{
	ax::Instance::init();
	ax::Instance::start();
	ax::Instance::deinit();

	return 0;
}