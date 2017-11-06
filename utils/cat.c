#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

static bool failed = false;

static void cat(const char* path)
{
	int fd;
	if (strcmp(path, "-") == 0)
	{
		fd = 0;
	}
	else
	{
		fd = open(path, O_RDONLY);
		if (fd < 0)
		{
			warn("'%s'", path);
			failed = true;
			return;
		}
	}
	
	while (true)
	{
		char buffer[1];
		ssize_t readSize = read(fd, buffer, sizeof(buffer));
		if (readSize < 0)
		{
			warn("'%s'", path);
			break;
		}
		else if (readSize == 0)
		{
			break;
		}

		ssize_t writtenSize = write(1, buffer, readSize);
		if (writtenSize < 0)
		{
			err(1, "write");
		}
	}
	if (fd != 0)
			close(fd);
}

int main(int argc, char* argv[])
{
	if (argc >= 2)
	{
		for (int i = 1; i < argc; i++)
		{
			cat(argv[i]);
		}
	}
	else
	{
		cat("-");
	}
	return failed ? 1 : 0;
}
