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

int set_gps_location_ext4(struct inode * now_node){
	return 0;
}

int get_gps_location_ext4(struct inode * now_node, struct gps_location * location){
	return 0;
}