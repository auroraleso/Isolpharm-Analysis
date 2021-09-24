#ifndef COMPASS_EVENT_H
#define COMPASS_EVENT_H

struct CompassEvent
{
	UShort_t channel;
	ULong_t  timestamp;
	UShort_t board;
	UShort_t energy;
	Int_t flags;
};

#endif
