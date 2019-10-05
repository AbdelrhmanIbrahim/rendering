#pragma once
namespace win
{
	//create window and set its callbacks 
	//(we have to do this as we can't bind member functions addresses as callbacks to glut)
	void
	window_create(unsigned int width, unsigned int height, const char* name);
};