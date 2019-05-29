#ifndef _REGION_H_
#define _REGION_H_
#include "pin.H"
#include <asm/unistd.h>
#define LEN_ADDR 8
// size changes with region num

typedef enum RegionType {
	GLOBAL = 0,
	HEAP,
	STACK,
	CODE,
	NO_REGION,
}RegionType;

typedef struct Region{
	RegionType type;
	unsigned base;
	unsigned end;
}Region;

class RegionInfo{
private:
	char region_type_str_[5][6]={"uknow","stack"," heap"," glob","code"};
	Region regions_[5];
public:
	RegionInfo();
	RegionType AddrRegion(ADDRINT addr);
	Region* GetRegions(){
		return regions_;
	}
};
#endif