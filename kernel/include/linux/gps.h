#ifndef _GPS_H
#define _GPS_H

struct gps_location {
	u64 latitude;
	u64 longitude;
	u32  accuracy;  /* in meters */
};

extern int set_gps_location_ext4(struct inode *);
extern int get_gps_location_ext4(struct inode *, struct gps_location *);


#endif