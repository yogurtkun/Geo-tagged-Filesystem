#include <linux/fs.h>
#include <linux/time.h>
#include <linux/jbd2.h>
#include <linux/highuid.h>
#include <linux/pagemap.h>
#include <linux/quotaops.h>
#include <linux/string.h>
#include <linux/buffer_head.h>
#include <linux/writeback.h>
#include <linux/pagevec.h>
#include <linux/mpage.h>
#include <linux/namei.h>
#include <linux/uio.h>
#include <linux/bio.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/ratelimit.h>
#include <linux/aio.h>
#include <linux/bitops.h>

#include <linux/gps.h>

#include "ext4_jbd2.h"
#include "xattr.h"
#include "acl.h"
#include "truncate.h"

#include <trace/events/ext4.h>

int set_gps_location_ext4(struct inode *inode)
{
	struct ext4_inode_info *ei;
	int coord_age;

	ei = EXT4_I(inode);
	read_lock(&location_lock);
	write_lock(&ei->gps_lock);
	/* TODO should i cast the type here? */
	coord_age = current_kernel_time().tv_sec - pos_time.tv_sec;
	memcpy(&ei->gps_info, &kernel_pos, sizeof(struct gps_location));
	memcpy(&ei->coord_age, &coord_age, sizeof(coord_age));
	read_unlock(&location_lock);
	write_unlock(&ei->gps_lock);

	return 0;
}

int get_gps_location_ext4(struct inode *inode,
			  struct gps_location *location)
{
	struct ext4_inode_info *ei;
	u32 coord_age;

	ei = EXT4_I(inode);

	write_lock(&location_lock);
	read_lock(&ei->gps_lock);
	memcpy(location, &ei->gps_info, sizeof(struct gps_location));
	memcpy(&coord_age, &ei->coord_age, sizeof(int));
	write_unlock(&location_lock);
	read_unlock(&ei->gps_lock);

	return coord_age;
}

int gps_test_opt(struct inode *inode)
{
	return test_opt(inode->i_sb, GPS_AWARE_INODE);
}
