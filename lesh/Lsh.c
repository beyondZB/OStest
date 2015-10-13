#include "read_command.h"
#include "map.h"
void main()
{
	init(&mapstring);	
	while(TRUE)
	{
		read_command();
	}

}
