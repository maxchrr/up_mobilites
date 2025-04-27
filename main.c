#include <stdio.h>
#include <stdlib.h>
#include "api.c"

int main(void)
{
	struct Bus_Stop* bs1 = create_bs(1,"Foo",10,20);
	struct Bus_Stop* bs2 = create_bs(2,"Bar",20,20);
	struct Bus_Route* br = create_br(1,bs1,bs2,1,1);
	struct Bus_Line* bl = create_bl(br);
	print_bl(bl, 0);
	free_bl(bl);
	free_br(br);
	free_bs(bs1);
	free_bs(bs2);
	return 0;
}
