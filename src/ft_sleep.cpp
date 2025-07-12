
/*
c++  ft_sleep.cpp &&  time (./a.out)
*/

#include <ctime>
#include <iostream>
#include "../include/All.hpp"
 
void ft_sleep(int nb)
{
	std::time_t start = std::time(0);
	while(1)
	{
		if (std::difftime(std::time(0), start) > nb)
			break;
	}
}

// int main()
// {
// 	ft_sleep(4);

// 	return(0);
// }


