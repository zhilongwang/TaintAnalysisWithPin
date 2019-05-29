#include "region.h"
#include "pin.H"

#define __DEBUG
#ifdef __DEBUG
#define DEBUG(format,...) printf(format,##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif
/* ===================================================================== */
/* funcions for getting region info */
/* ===================================================================== */

RegionInfo::RegionInfo()
{
	INT pid = PIN_GetPid();
	//printf("Pid : %d\n", pid);

	FILE *procmaps;
	char path[20];
	char line[100];

	sprintf(path, "/proc/%d/maps", pid);

	procmaps = fopen(path, "r"); 
	int timesdata = 0;
	int timescode = 0;
	while(fgets(line, 100, procmaps) != NULL) {
		//printf("%s", line);
		if (timescode == 0 && strstr(line, "r-xp")) {
			sscanf(strndup(line, 1+2*LEN_ADDR), "%x-%x", &regions_[CODE].base, &regions_[CODE].end);
			DEBUG("CODE REGION:%x and %x\n", regions_[CODE].base, regions_[CODE].end);
			regions_[CODE].type = CODE;
			timescode++;
		}
		if (timesdata == 0 && strstr(line, "rwxp")) {
			sscanf(strndup(line, 1+2*LEN_ADDR), "%x-%x", &regions_[GLOBAL].base, &regions_[GLOBAL].end);
			DEBUG("DATA REGION:%x and %x\n", regions_[GLOBAL].base, regions_[GLOBAL].end);
			regions_[GLOBAL].type = GLOBAL;
			timesdata++;
		}
		if (strstr(line, "[stack")) {
			sscanf(strndup(line, 1+2*LEN_ADDR), "%x-%x", &regions_[STACK].base, &regions_[STACK].end);
			regions_[STACK].type = STACK;
			DEBUG("STACK REGION:%x and %x\n", regions_[STACK].base, regions_[STACK].end);
		}
		if (strstr(line, "[heap")) {
			sscanf(strndup(line, 1+2*LEN_ADDR), "%x-%x", &regions_[HEAP].base, &regions_[HEAP].end);
			regions_[HEAP].type = HEAP;
			DEBUG("HEAP REGION:%x and %x\n", regions_[HEAP].base, regions_[HEAP].end);
		} 
	} 
	//KNOB<BOOL>   KnobNoCompress(KNOB_MODE_WRITEONCE, "pintool","no_compress", "0", "Do not compress");

	//fprintf(trace, "%x;%x\n", regions_[GLOBAL].base, regions_[GLOBAL].end);
	//fprintf(trace, "%x;%x\n", regions_[STACK].base, regions_[STACK].end);
	//fprintf(trace, "%x;%x\n", regions_[HEAP].base, regions[HEAP].end);
}
RegionType RegionInfo::AddrRegion(ADDRINT addr){
	RegionType type=NO_REGION;
	if(addr<regions_[STACK].end && addr>regions_[STACK].base){
		type=STACK;
	}else if(addr<regions_[HEAP].end && addr>regions_[HEAP].base){
		type=HEAP;
	}else if(addr<regions_[GLOBAL].end && addr>regions_[GLOBAL].base){
		type=GLOBAL;
	}
	return type;
}
