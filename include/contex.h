#ifndef CONTEX_H
#define CONTEX_H
#endif

string invalid = "invalid_rtn";
string *ADDRINT2str(ADDRINT value){
	char str[16];
    	sprintf(str,"%x",(unsigned int)value);
	return new string(str);
}
/* ===================================================================== */
const string *Target2String(ADDRINT target)
{
    string name = RTN_FindNameByAddress(target);
    if (name == "")
        return &invalid;
    else
        return new string(name);
}
