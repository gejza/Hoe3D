
#ifndef _HOE_CORE_TEST_H_
#define _HOE_CORE_TEST_H_

namespace HoeTest {

enum ETestStatus
{
	TEST_OK,
	TEST_WARNING,
	TEST_FAILED,
};

enum ELogInfo
{
};

class BaseTest
{
	static const int maxtests = 1000;
	static BaseTest *s_tests[maxtests];
protected:
	const char * m_name;
	const char * m_desc;
public:
	BaseTest(const char * name, const char * desc);
	virtual ETestStatus Run() = 0;

	static int RunAll();
	static int RunTest(const char * name);
};

#define DEFINE_TEST(name, desc) class name : public HoeTest::BaseTest \
	{ \
	public: \
	name() : HoeTest::BaseTest(#name, desc) {} \
	virtual HoeTest::ETestStatus Run()

#define END_TEST(name) \
}; \
static name test##name;

/*/ pouziti 1
DEFINE_TEST(Test, "Decription of test");
// dalsi definice
END_TEST(Test)

// funkce
int Test::run()
{
	return TEST_OK;
}
/*/
/*/ pouziti 2
DEFINE_TEST(Test, "Decription of test")
{
	return TEST_OK;
}
// dalsi definice
END_TEST(Test)

/*/

} // namespace HoeTest

#endif // _HOE_CORE_TEST_H_
