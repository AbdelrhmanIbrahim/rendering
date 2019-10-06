#include "app/app.h"

int
main(int argc, char** argv)
{
	//build on heap (size -- revisit)
	app::application app(argc, argv);
	app.run();
	return 0;
}