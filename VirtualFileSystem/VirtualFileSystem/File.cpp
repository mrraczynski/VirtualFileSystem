#include "IVFS.h"

namespace TestTask
{
	File::File(const char* nm)
	{
		strcpy_s(name, strlen(name), nm);
		length = 0;
		startPoint = 0;
		realFileName[0] = '\0';
	}
}
