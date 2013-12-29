#include <stdio.h>
#include <stdlib.h>
#include <libudev.h>
#include <fcntl.h>

int main() 
{
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

			printf("Got Device\n");
			printf("   Node: %s\n", udev_device_get_devnode(dev));
			printf("   Subsystem: %s\n", udev_device_get_subsystem(dev));
			printf("   Devtype: %s\n", udev_device_get_devtype(dev));

			printf("   Action: %s\n",udev_device_get_action(dev));
		} else {
			puts("An error occured.");
		}
	}
	puts("Hello world.");
	return 0;
}
