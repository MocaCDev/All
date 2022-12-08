#include "ecbp_file_format.h"

int main(int args, char **argv)
{
	_ECBP_format *f = calloc(1, sizeof(*f));

	f->start = 'p';

	char *name = "package_name";
	memcpy(f->pkg_name, name, strlen(name));
	f->pkg_name[14] = '\0';
	f->pkg_size = 0x24;

	FILE* fl = fopen("test.ecbp", "wb");

	fwrite(f, 1, sizeof(struct ECBP_format), fl);
	fclose(fl);
	return 0;
}
