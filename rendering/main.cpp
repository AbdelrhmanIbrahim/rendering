#include "app/app.h"

#include "IO/Image.h"

int
main(int argc, char** argv)
{
	app::application app(argc, argv);
	app.run();
	return 0;
}