#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
	char* fast = NULL;
	char* speed = NULL;
	int x;
	for(x = 1;x < argc;x++)
	{
		if(argv[x][0] == '-')
		{
			if(argv[x][1] == 's')
			{
				speed = argv[x + 1];
				x++;
			}
		} else {
			if(fast == NULL)
				fast = argv[x];
		}
	}

	double cpu_speed = atof(speed);
	double cycles = (double)atoi(fast);
	/* Adjust cpu speed */
	cpu_speed *= 1000000;

	if(cpu_speed == 0) return -1;
	double result = (cycles * 1000000000.00f) / cpu_speed;

	result *= 10;
	result = (long long int)result;
	result /= 10;

	printf("%lf\n", result);
	return 0;
}
