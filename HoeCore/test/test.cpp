
#include "../src/StdAfx.h"
#include <stdlib.h>
#include "../include/hoe_core.h"
#include "../include/hoe_structures.h"
#include "test.h"

//#include <sys/time.h>

////////// Timer /////////////////

/*float SysFloatTime()
{
	struct timeval ctime;

	gettimeofday(&ctime, NULL);

	return ((ctime.tv_sec % 100000) + ctime.tv_usec * 0.000001f);
}*/

HoeTest::BaseTest *HoeTest::BaseTest::s_tests[HoeTest::BaseTest::maxtests] = {0};

HoeTest::BaseTest::BaseTest(const char * name, const char * desc)
{
	m_name = name;
	m_desc = desc;
	for (int i=0;i < maxtests;i++)
	{
		if (!s_tests[i])
		{
			s_tests[i] = this;
			return;
		}
	}
	// assert
	throw;
}

int HoeTest::BaseTest::RunAll()
{
	int nok = 0, nfail = 0, nwarn = 0;

	for (int i=0;i < maxtests;i++)
	{
		if (!s_tests[i])
			break;
		BaseTest* t = s_tests[i];

		printf("=== %2d: Running: %s (%s) ===\n", i+1, t->m_name, t->m_desc);
		switch (t->Run())
		{
		case TEST_OK:
			printf("OK\n");
			nok++;
			break;
		case TEST_WARNING:
			printf("WARNING\n");
			nok++;nwarn++;
			break;
		case TEST_FAILED:
			printf("FAILED\n");
			nfail++;
			break;
		default:
			hoe_assert(!"unknown value");
		};
	}
	if (!nfail && !nwarn)
		printf("!Excelent work! - %d tests ok.\n", nok);
	else {

		printf("Summary: From %d tests is only %d%% ok. (ok: %d fail:%d warn: %d)\n",
			nok+nfail,100*nok/(nok+nfail),nok-nwarn,nfail,nwarn);
		return 1;
	}
	return 0;
}

int HoeTest::BaseTest::RunTest(const char* test)
{
	int nok = 0, nfail = 0, nwarn = 0;

	for (int i=0;i < maxtests;i++)
	{
		if (!s_tests[i])
			break;
		BaseTest* t = s_tests[i];

		if (HoeCore::string::cmp(t->m_name, test))
			continue;
		return t->Run();
	}
	return 0;
}

int main()
{
	//return HoeTest::BaseTest::RunAll();
	return HoeTest::BaseTest::RunTest("List");
}


