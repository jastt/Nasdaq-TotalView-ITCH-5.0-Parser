#pragma once
#include "NQTV.h"
#include "Includes.h"


class  CNQTVFeed
{
public: /*: public Database::TickerTable*/

	 CNQTVFeed(void);
	virtual ~ CNQTVFeed(void);

private:        // the default anyway


public:	
	FEED_MESSAGE_STATS GetStats();
	FEED_MESSAGE_STATS m_MESSAGE_STATS;


};

