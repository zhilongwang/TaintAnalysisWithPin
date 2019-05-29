#ifndef _RT_CHECK_H_
#define _RT_CHECK_H_
#include "pin.H"

#include<string>
#include<iostream>
#include<list>
using std::cout;
using std::string;
using std::hex;

typedef enum SinkType{
    CONTROL_DEPENDENCY_SINK =  0,
    INDIRECT_CALL_SINK,
    UNDEFINED_SINK,
}SinkType;
typedef struct SinkPoint{
    enum SinkType type;
    UINT32 addr;
    bool operator==(struct SinkPoint b) const  
	{  
		return this->addr == b.addr;
	}  
	bool operator!=(struct SinkPoint b) const  
	{  
		return this->addr != b.addr;
	} 
	bool operator>=(struct SinkPoint b) const  
	{  
		return this->addr >= b.addr;
	} 
	bool operator<=(struct SinkPoint b) const  
	{  
		return this->addr <= b.addr;
	} 
	bool operator>(struct SinkPoint b) const  
	{  
		return this->addr > b.addr;
	} 
	bool operator<(struct SinkPoint b) const  
	{  
		return this->addr < b.addr;
	} 
}SinkPoint;

VOID CheckIndirectCall(VOID * ip, string assemble, REG reg_1);

VOID CheckOneReg(VOID * ip, string assemble, REG reg_1);
VOID CheckTwoReg(VOID * ip, string assemble, REG reg_1, REG reg_2);
VOID CheckOneRegOneMem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG reg_1);
VOID CheckTwoRegOneMem(VOID * ip, string assemble, VOID * addr, UINT32 size, REG reg_1, REG reg_2);
class SinkInfo{
private:
    std::list<SinkPoint> sink_points_;
    static string sink_type_name[];
public:
    string GetSinkTypeName(enum SinkType type){
        return sink_type_name[type];
    }
    bool AddSinkPoint(enum SinkType type, UINT32 ip){
        SinkPoint sink_point={type, ip};
		//taintCmp.push_back(binst);
        sink_points_.push_back(sink_point);
        return true;
    }
    bool PrintSinkInfo(){
        list<SinkPoint>::iterator it;
        sink_points_.sort();
        sink_points_.unique();
        for(enum SinkType t=CONTROL_DEPENDENCY_SINK; t<UNDEFINED_SINK; t=(SinkType)(t+1)){
            std::cout << "\n\tType: " << GetSinkTypeName(t) << "Sink Information"<< endl;
            UINT32 format_control = 0;
            for(it = sink_points_.begin(); it != sink_points_.end(); it++){
                SinkPoint sink_point = *it;
                if(sink_point.type == t){
                    std::cout << std::hex<<"\t"<< sink_point.addr;
                    format_control++;
                    if(format_control==8){
                        format_control=0;
                        std::cout << std::endl;
                    }
                }

            }
        }
        std::cout << std::endl;
        return true;
    }
};

#endif