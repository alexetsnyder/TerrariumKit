#include "MainGame.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
	MainGame mainGame{};
	mainGame.run();

	std::cout << "Hello World!\n";
	int a;
	std::cin >> a;

	return EXIT_SUCCESS;
}