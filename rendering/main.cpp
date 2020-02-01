#include "app/app.h"

int
main(int argc, char** argv)
{
	app::application app(argc, argv);
	while (1)
	{
		app.run();
	}
	return 0;
}