#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


int check_root_dir(const char* path)
{
	struct stat buf;
	if (stat(path, &buf) == -1)
	{
		perror("stat");
		return -1;
	}

	if (S_ISDIR(buf.st_mode) == 0)
	{
		printf("Not a directory\n");
		return -1;
	}

	if (((S_IXUSR & buf.st_mode) == 0) || ((S_IXOTH & buf.st_mode) == 0)) 
	{
		printf("Couldn't access directory\n");
		return -1;
	}

	return 0;
}

int check_and_open(const char *url, const char *document_root)
{
	char full_url[256];
	sprintf(full_url, "%s/%s", document_root, url);

	if (access(full_url, R_OK) == -1)
	{
		perror(full_url);
		return -1;
	}
	
	return open(full_url, O_RDONLY);
}
