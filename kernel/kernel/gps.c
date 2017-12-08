#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/wait.h>
#include <linux/gps.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/time.h>

static struct gps_location kernel_pos;
static struct timespec pos_time;
static DEFINE_RWLOCK(location_lock);

SYSCALL_DEFINE1(set_gps_location,struct gps_location __user *,loc){
	struct gps_location temp_loc;

	if (current_uid() != 0)
		return -EACCES;

	if(copy_from_user(&temp_loc,loc,sizeof(struct gps_location)))
		return -EINVAL;

	printk("%llx %llx %x\n!!!",temp_loc.latitude,temp_loc.longitude,temp_loc.accuracy);

	write_lock(&location_lock);
	memcpy(&kernel_pos,&temp_loc,sizeof(struct gps_location));
	getnstimeofday(&pos_time);
	write_unlock(&location_lock);

	return 0;
}

SYSCALL_DEFINE2(get_gps_location, const char __user *, pathname,
		     struct gps_location __user *, loc){
	return 0;
}