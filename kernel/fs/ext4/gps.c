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

// extern struct gps_location kernel_pos;
// extern struct timespec pos_time;
// extern rwlock_t location_lock;
// 
// struct gps_location {
// 	u64 latitude;
// 	u64 longitude;
// 	u32  accuracy;  /* in meters */
// };

int set_gps_location_ext4(struct inode *inode){
	struct ext4_inode_info *ei;
	int coord_age;

	ei = EXT4_I(inode);
	read_lock(&location_lock);
	write_lock(&ei->gps_lock);
	/* TODO should i cast the type here? */
	coord_age = current_kernel_time().tv_sec - pos_time.tv_sec;
	memcpy(&ei->gps_info,&kernel_pos,sizeof(struct gps_location));
	memcpy(&ei->coord_age,&coord_age,sizeof(coord_age));
	read_unlock(&location_lock);
	write_unlock(&ei->gps_lock);

	return 0;
}

// int set_gps_location_ext4(struct inode *inode)
// {
// 	int error = 0;
// 	struct ext4_iloc iloc;
// 	struct ext4_inode *raw_inode;
// 	struct ext4_inode_info *ei;
// 	/* TODO how should I specify that it is a 32-bit? */
// 	int32_t coord_age;

// 	/* get the raw inode */
// 	error = ext4_get_inode_loc(inode, &iloc);
// 	if (error)
// 		goto skip_set;

// 	raw_inode = ext4_raw_inode(&iloc);
// 	if (!raw_inode) {
// 		error = -ENODEV;
// 		goto skip_set;
// 	}

// 	read_lock(&location_lock);
// 	/* copy to raw_inode */
// 	 Q: whether raw inode need lock?
// 	 * A: it seems that before calling the function, the lock will be set
// 	 * 
// 	/* TODO should i cast the type here? */
// 	coord_age = current_kernel_time().tv_sec - pos_time.tv_sec;
// 	raw_inode->i_latitude = cpu_to_le64(kernel_pos.latitude);
// 	raw_inode->i_longitude = cpu_to_le64(kernel_pos.longitude);
// 	raw_inode->i_accuracy = cpu_to_le32(kernel_pos.accuracy);
// 	raw_inode->i_coord_age = cpu_to_le32(coord_age);
// 	read_unlock(&location_lock);
	
// 	/* The common practice(s) */
// 	brelse(iloc.bh);
// 	ext4_set_inode_flags(inode);
// 	unlock_new_inode(inode);
// 	return error;
// skip_set:
// 	brelse(iloc.bh);
// 	iget_failed(inode);
// 	return error;
// }

int get_gps_location_ext4(struct inode * inode, struct gps_location * location){
	struct ext4_inode_info *ei;

	ei = EXT4_I(inode);

	write_lock(&location_lock);
	read_lock(&ei->gps_lock);
	memcpy(location,&ei->gps_info,sizeof(struct gps_location));
	write_unlock(&location_lock);
	read_unlock(&ei->gps_lock);

	return 0;	
}

int gps_test_opt(struct inode *inode){
	return test_opt(inode->i_sb, GPS_AWARE_INODE);
}
