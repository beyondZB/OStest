void _exit();
void _start()
{
	main();
	_exit();
}

int main()
{
	ttyOutputStr("this is output 1 in process!!");
	ttyOutputStr("This is a user process!!");
	return 0;
}

void _exit()
{
	ttyOutputStr(" _exit ");
	exitProcess();
}
