#include "SGXEpg.h"


SGXEpg::SGXEpg () {
}

SGXEpg::~SGXEpg () { }

/**
* @param  movie_id
*/

bool SGXEpg::isFreeToPlay(size_t movie_id)
{
	// parsing the data inside the enclave, 
	// The first time in the arena, a deadly stunt, hurry to see !
	unsigned char *ptr = (unsigned char*)getContent();
	unsigned char *end = ptr + get_data_size() + 1; // including trailing zero
	do
	{
		unsigned char *name = ptr;
		unsigned char *chid = (unsigned char*)strchr((const char*)name, ',') + 1; if (chid >= end) return false;
		unsigned char *tm = (unsigned char*)strchr((const char*)chid, ',') + 1; if (tm >= end) return false;
		unsigned char *stat = (unsigned char*)strchr((const char*)tm, ',') + 1; if (stat >= end) return false;

		if (strncmp((char*)stat, "free view", strlen("free view")) == 0)
		{
			// getting id
			size_t id = atoll((const char*)chid);
			if (id == movie_id) return true; // if its state is tree view, return true.
		}
		ptr = (unsigned char*)strchr((const char*)ptr, '\n') + 1;
	} while (ptr < end);

	return false;
}

/**
* @return bool
* @param  data_1024_
* @param  pageNum
*/

bool SGXEpg::getEPGPage(unsigned char data[1024], int pageNum)
{
	memcpy(data, this->getContent() + pageNum * 1024, 1024);
	return true;
}

bool SGXEpg::download()
{
	return downloadById("epg");
}

