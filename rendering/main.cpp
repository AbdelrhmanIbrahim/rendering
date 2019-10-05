#include "app/app.h"

using namespace app;

int
main(int argc, char** argv)
{
	application app(argc, argv);
	app.run();
	return 0;
}