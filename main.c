#include <stdio.h>
#include <stdlib.h>
#include <libudev.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void daemonise()
{
	pid_t pid, sid;

	/* Are we already a daemon? */
	if (getpid() == 1)
		return;

	/* Fork from the parent process. */
	pid = fork();
	if (pid < 0)
		exit(1);
	/* If we have a good pid, exit to parent process. */
	if (pid > 0)
		exit(0);

	/* We are now executing as the child process. */

	/* Change the file mode mask. */
	umask(0);

	/* Create a new SID for the child process. */
	sid = setsid();
	if (sid < 0)
		exit(1);

	/* Change working directory. */
	if (chdir("/") < 0)
		exit(1);

	/* Redirect standard files to /dev/null. */
	freopen( "/dev/null", "r", stdin);
	freopen( "/dev/null", "w", stdout);
	freopen( "/dev/null", "w", stderr);	
}

int main() 
{
	openlog("autodeathd", LOG_PID|LOG_CONS, LOG_USER);
	struct udev *udev;
	udev = udev_new();

	if (!udev) {
		puts("Cannot create initialise udev.");
		exit(1);
	}

	struct udev_monitor *mon;

	/* TODO "kernel" may be more appropiate for our requirments. */
	mon = udev_monitor_new_from_netlink(udev, "udev");
	udev_monitor_enable_receiving(mon);

	/* Turn on blocking.  */
	int fd = udev_monitor_get_fd(mon);
	int flags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);

	struct udev_device *dev;

	for (;;) {

		dev = udev_monitor_receive_device(mon);

		if (dev) {

			/*TODO is there a better way to filter the action? */
			if (!strcmp(udev_device_get_action(dev), "add")) {

				syslog(LOG_INFO, "   Node: %s\n"
					"\tSubsystem: %s\n"
					"\tDevtype: %s\n", 
					udev_device_get_devnode(dev),
					udev_device_get_subsystem(dev),
					udev_device_get_devtype(dev));
			}
		} else {
			puts("An error occured.");
		}
	}

	closelog();
	return 0;
}
