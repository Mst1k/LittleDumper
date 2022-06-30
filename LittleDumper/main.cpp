#include "LittleDumper.h"
#include <iostream>
int main()
{
	LittleDumper* littleDumper = new LittleDumper();

	if (littleDumper->Init())
		littleDumper->Run();
	else return 1;

	std::cout << "Dump Done!\n";

	return 0;
}
