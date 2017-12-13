#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include "gpsd.h"

#define TIME_INTERVAL 1000000

void daemon_mode(void)
{
	pid_t pid, sid;

	pid = fork();

	if (pid < 0) {
		printf("Fork failed\n");
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	int flog =
	    open("./gpslog.txt", O_WRONLY | O_APPEND | O_CREAT);

	sid = setsid();
	if (sid < 0) {
		printf("Set sid failed\n");
		exit(EXIT_FAILURE);
	}

	if (chdir("/") < 0) {
		printf("Change directory failed\n");
		exit(EXIT_FAILURE);
	}

	dup2(flog, 1);
	close(0);
	close(2);
}

int read_gps_data(void)
{
	FILE *fout;
	fout = fopen(GPS_LOCATION_FILE, "r");
	if (!fout) {
		printf("File open failed\n");
		return EXIT_FAILURE;
	}

	struct gps_location loc;
	int n = fscanf(fout, "%lf %lf %f", &loc.latitude, &loc.longitude, &loc.accuracy);
	if (n != 3) {
		printf("Read data from file failed\n");
		fclose(fout);
		return EXIT_FAILURE;
	}

	int ret = syscall(__NR_set_gps_location,&loc);
	if (ret < 0){
		printf("Write data to kernel failed\n");
		fclose(fout);
		return EXIT_FAILURE;
	}

	fclose(fout);
	return 0;

}

int main(int argc, char *argv[])
{
	/* deamon mode */
	int err = 0;

	while(1) {
		err = read_gps_data();
		if(err)
			break;
		usleep(TIME_INTERVAL);
	}

	printf("Deamon process exit!\n");
	
	return err;
}
