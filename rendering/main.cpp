#include "app/app.h"

int
main(int argc, char** argv)
{
	app::application app(argc, argv);
	app.run();
	return 0;
}