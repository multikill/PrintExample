#include "Print.hpp"

int main()
{
	TXT("I'm text in the file! %i", 42);
	CONSOLE("I'm text in the normal console! %i", 1337);
}