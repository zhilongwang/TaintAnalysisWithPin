# TaintAnalysisWithPin
Taint analysis with pin tool.


### Install


```
git clone --recurse-submodules https://github.com/zhilongwang/TaintAnalysisWithPin.git
cd TaintAnalysisWithPin 
make
```

### Run test case.
```
# current we only support x86 binary.
gcc -o test/demo test/demo.c -m32 
./pin/pin -t obj-ia32/taint.so -- ./test/demo comandlinepara
```

