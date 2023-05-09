#include "MovingWindow.h"
#include <tuple>
#include <iostream>

int main()
{
	MovingWindow window(1920, 1080, "Pathfinder Visualisation", 1000);
	window.run();

	return 0;
}