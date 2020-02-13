#include "app/app.h"

using namespace app;

int
main(int argc, char** argv)
{
	App app = app_new();
	app_run(app);
	app_free(app);
	return 0;
}