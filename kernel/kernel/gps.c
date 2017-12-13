#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/wait.h>
#include <linux/gps.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <linux/namei.h>

struct gps_location kernel_pos = {
	.latitude = 0,
	.longitude = 0,
	.accuracy = 0,
};

struct timespec pos_time;
DEFINE_RWLOCK(location_lock);

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
	struct gps_location temp_loc;

	if (current_uid() != 0)
		return -EACCES;

	if (copy_from_user(&temp_loc, loc, sizeof(struct gps_location)))
		return -EINVAL;

	write_lock(&location_lock);
	memcpy(&kernel_pos, &temp_loc, sizeof(struct gps_location));
	pos_time = current_kernel_time();
	write_unlock(&location_lock);

	return 0;
}

SYSCALL_DEFINE2(get_gps_location, const char __user *, pathname,
		struct gps_location __user *, loc)
{

	struct path path;
	unsigned int lookup_flags = LOOKUP_FOLLOW;
	struct inode *inode;
	struct gps_location temp_loc;
	int res;

	res = user_path_at(AT_FDCWD, pathname, lookup_flags, &path);
	inode = path.dentry->d_inode;
	if (res)
		return -EINVAL;

	res = inode_permission(inode, MAY_READ);
	if (res)
		return -EINVAL;

	if (!gps_test_opt(inode) || !(inode->i_op)
	    || !(inode->i_op->get_gps_location))
		return -ENODEV;
	res = inode->i_op->get_gps_location(inode, &temp_loc);
	if (copy_to_user(loc, &temp_loc, sizeof(struct gps_location)))
		return -EINVAL;


	return res;
}
