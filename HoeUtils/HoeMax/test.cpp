// test.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "ExportFile.h"

int main(int argc, const char * argv[])
{
	ModelExportFile f;
	f.Load(L"test.txt");
	return 0;
}

