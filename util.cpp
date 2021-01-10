#include "util.h"

float randDirection()
{
	if (rand() % 100 > 50) {
		return 1;
	}
	else return -1;
}
