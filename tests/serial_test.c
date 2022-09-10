#define _XOPEN_SOURCE 600
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "io/serial_test.h"


#define _GNU_SOURCE
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

pid_t child_pid = -1;
char pty1_pts_name[256];
char pty2_pts_name[256];
int pty1_file_descriptor;
int pty2_file_descriptor;
const int buffer_io = 4096;
int pipe_fd[2];

void fork_socat(void);
void kill_socat(void);
void psuedoterminal(void);
void close_psuedoterminal(void);
void swap(off_t *num1,off_t *num2);
void link_terminals(void);

void psuedoterminal()
{
	int result;

	memset(pty1_pts_name,0,256);
	memset(pty2_pts_name,0,256);
	pty1_file_descriptor = posix_openpt(O_RDWR | O_NOCTTY);
	pty2_file_descriptor = posix_openpt(O_RDWR | O_NOCTTY);
	if (pty1_file_descriptor != -1)
	{
		result = grantpt(pty1_file_descriptor);
		result = unlockpt(pty1_file_descriptor);
		result = ptsname_r(pty1_file_descriptor,pty1_pts_name,256);
		printf("pts_name = %s\n",pty1_pts_name);
	}
	if (pty2_file_descriptor != -1)
	{
		result = grantpt(pty2_file_descriptor);
		result = unlockpt(pty2_file_descriptor);
		result = ptsname_r(pty2_file_descriptor,pty2_pts_name,256);
		printf("pts_name = %s\n",pty2_pts_name);
	}
	
	result = pipe(pipe_fd);
	if (result < 0)
	{
		perror("pipe creation error\n");
		close_psuedoterminal();
		return;
	}
}

void link_terminals()
{
	int result;
	off_t in_off;
	off_t out_off;
	off_t pty1_len;
	off_t pty1_buffer_size;
	off_t pty2_buffer_size;
	off_t pty2_len;
	struct stat stat_buffer;

	in_off = 0;
	out_off = 0;
	pty1_buffer_size = buffer_io;
	pty2_buffer_size = buffer_io;
	if (pty1_file_descriptor != -1 && pty2_file_descriptor != -1)
	{
		result = fstat(pty1_file_descriptor,&stat_buffer);
		if (result < 0)
		{
			perror("fstat error pty1\n");
			close_psuedoterminal();
			return;
		}
		pty1_len = stat_buffer.st_size;
		while (pty1_len > 0)
		{
			
		}
		result = fstat(pty2_file_descriptor,&stat_buffer);
		if (result < 0)
		{
			perror("fstat error pty2\n");
			close_psuedoterminal();
			return;
		}
		pty2_len = stat_buffer.st_size;
		if (len > higher_len)
		{
			swap(&len,&higher_len);
		}
		while (higher_len > 0)
		{
			if (len > higher_len)
				len = higher_len;
			result = splice(pty1_file_descriptor,&in_off,pipe_fd[1],NULL,len, SPLICE_F_MOVE | SPLICE_F_MORE);
			if (result < 0)
			{
				perror("splice error on pty1\n");
				close_psuedoterminal();
				return;
			}
			result = splice(pipe_fd[0],NULL,pty2_file_descriptor,&out_off,len, SPLICE_F_MOVE | SPLICE_F_MORE);
			if (result < 0)
			{
				perror("splice error on pty2\n");
				close_psuedoterminal();
				return;
			}

		}
	}

}

void close_psuedoterminal()
{
	if (pty1_file_descriptor != -1)
		close(pty1_file_descriptor);
	if (pty2_file_descriptor != -1)
		close(pty2_file_descriptor);
	pty1_file_descriptor = -1;
	pty2_file_descriptor = -1;
}

void swap(off_t *num1, off_t *num2)
{
	off_t tmp;

	tmp = *num1;
	*num1 = *num2;
	*num2 = tmp;
}

void fork_socat (void)
{
	child_pid = fork();
	if (child_pid == 0)
	{
		FILE *fp;
		char output[1024];
		int res;
		regex_t regex;

		fp = popen ("socat -d -d pty,raw,echo=0 pty,raw,echo=0 2>&1","r");
		if ( fp )
		{
			fgets(output, 1024, fp);
			printf("child %i\n",getpid());
			printf("%s\n",output);
			pclose(fp);
		}
	}
	else if (child_pid != -1)
	{
		sleep(5);
		const struct CMUnitTest tests[] = {};
		cmocka_run_group_tests_name("serial transfer",tests, NULL, NULL);
		printf("kill %i -> %i\n",child_pid,getpid());
		kill(child_pid, SIGKILL);
		//wait(NULL);
	}
}

void kill_socat (void)
{
	if (child_pid > 0)
		kill (child_pid, SIGKILL);
}



int main(void) 
{
    const struct CMUnitTest tests[] = {
    cmocka_unit_test(serial_device_new),
	cmocka_unit_test_setup(serial_destroy,serial_setup),
	cmocka_unit_test_setup_teardown(serial_set_baudrate,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_parity,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_access_mode,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_handshake,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_local_echo,serial_setup,serial_teardown),	
	cmocka_unit_test_setup_teardown(serial_set_databits,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_set_stopbits,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_configure,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_configure_115200_8N1,serial_setup,serial_teardown),
	cmocka_unit_test_setup_teardown(serial_configure_9600_8N1,serial_setup,serial_teardown),
    };
    cmocka_run_group_tests_name("serial",tests, NULL, NULL);
	//fork_socat();
	psuedoterminal();
    return 0;
}
