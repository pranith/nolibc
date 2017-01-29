#ifndef NOLIBC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#endif

static const char *ltoa(long in)
{
	static char buffer[30];

        char *pos;
        int neg = 0;
        unsigned long n;

        pos = buffer + sizeof(buffer) - 1;
        *pos-- = '\0';

        if (in < 0) {
                neg = 1;
                n = -in;
        }
        else
                n = in;

        do {
                *pos-- = '0' + n % 10;
                n /= 10;
        } while (n && pos >= buffer);

        if (neg && pos > buffer)
                *pos-- = '-';
        return pos + 1;
}

void writestr(int fd, const char *str)
{
	write(fd, str, strlen(str));
}

void writeint(int fd, unsigned long x)
{
	writestr(fd, ltoa(x));
}

/* prints "$hdr: <$val>\n" */
void print_int(const char *hdr, int val)
{
	writestr(1, hdr);
	writestr(1, ": <");
	writeint(1, val);
	writestr(1, ">\n");
}

/* prints "$hdr: <$val>\n" */
void print_str(const char *hdr, const char *val)
{
	writestr(1, hdr);
	writestr(1, ": <");
	writestr(1, val);
	writestr(1, ">\n");
}

int main(int argc, char **argv, char **envp)
{
	const char *env = envp[0] ? envp[0] : "none";
	int ret;
	int err;

	print_int("pid ", getpid());

	writestr(1, "msg : <");
	ret = write(1, "hello", 5);
	err = errno;
	writestr(1, ">\n");

	print_int("ret ", ret);
	print_int("err ", err);
	print_int("argc", argc);
	print_str("argv", argv[0]);
	print_str("envp", env);
	return 0;
}
