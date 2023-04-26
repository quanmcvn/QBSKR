#include <memory>
#include "qbskr/main.hpp"

static std::unique_ptr<Main> g_main;

int main(int argc, char** argv)
{
	g_main = std::make_unique<Main>();
	int ret = g_main->run(argc, argv);

	g_main.reset();

	return ret;
}