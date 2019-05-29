#ifndef _REGISTER_H_
#define _REGISTER_H_
#include "pin.H"
#include<string>
using std::string;

string GetREGName(REG reg);
class ShadowReg{
private:
    bool shadow_reg_[287]={false};
public:

    bool checkREG(REG reg);
    bool taintREG(REG reg);
    bool removeREG(REG reg);
};

#endif