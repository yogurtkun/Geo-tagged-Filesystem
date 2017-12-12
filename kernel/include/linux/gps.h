#ifndef _GPS_H
#define _GPS_H

struct gps_location {
	u64 latitude;
	u64 longitude;
	u32  accuracy;  /* in meters */
};

extern struct gps_location kernel_pos;
extern struct timespec pos_time;
extern rwlock_t location_lock;

#endif
