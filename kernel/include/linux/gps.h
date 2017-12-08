#ifndef _GPS_H
#define _GPS_H

struct gps_location {
	u64 latitude;
	u64 longitude;
	u32  accuracy;  /* in meters */
};

#endif