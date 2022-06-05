#include "IVFS.h"

namespace TestTask
{
	File::File(const char* nm, long int len)
	{
		strcpy_s(name, strlen(name), nm);
		length = len;
	}
}
