#ifndef _ITEM_H_
#define _ITEM_H_

typedef struct
{
	int x;
	int y;
}Item_location_t;

class Item
{
private:
	int item_number;
	Item_location_t location;
	int repitation;

public:
	Item(int item_number, Item_location_t item_location, int item_repitations)
	{
		this->item_number = item_number;
		location = item_location;
		repitation = item_repitations;
	};
};
#endif