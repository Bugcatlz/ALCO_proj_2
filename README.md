ALO_proj_2
=== 
## Global Variable
### A. registers
```c++
vector <Register*> registers(32);
```
為一個vector，用來儲存指向Register的pointer，

其代表的意義為代表32個的register

## Class
### A. Instruction
### Private:
### 1. myReg
```c++
vector < Register* > myReg
```
用來存放這個Instruction所使用到的Register

### 2.  myAddress
```c++
int myAddress
```
用來存放這個Instruction的位置

### 3. myOperator
```c++
string myOperator
```
用來存放這個Instruction的operator

### 4. myImmediate
```c++
int myImmediate
```
用來存放這個Instruction中immediate的值

### 5. myLabel
```c++
string myLabel
```
用來存放這個Instruction中的Label的名子

### Public
### 1. Constructor
```c++
Instruction(int address, string myOperator, Register* reg1, Register* reg2, Register* reg3)
	:myAddress(address),
	myOperator(myOperator)
{
	myReg.push_back(reg1);
	myReg.push_back(reg2);
	myReg.push_back(reg3);
}
```
為用來建構一個Instruction包含三個Register的Constructor

Ex: add R6,R5,R4

```C++
Instruction(int address, string myOperator, Register* reg1, Register* reg2, int imm)
	:myAddress(address),
	myOperator(myOperator),
	myImmediate(imm)
{
	myReg.push_back(reg1);
	myReg.push_back(reg2);
}
```
為用來建構一個Instruction包含二個Register和一個Immediate的Constructor

Ex: addi R5,R5,1

```C++
Instruction(int address, string myOperator, Register* reg1, int imm)
	:myAddress(address),
	myOperator(myOperator),
	myImmediate(imm)
{
	myReg.push_back(reg1);
}
```

為用來建構一個Instruction包含一個Register和一個Immediate的Constructor

Ex: li R3,16

```C++
Instruction(int address, string myOperator, Register* reg1, Register* reg2, string label)
	:myAddress(address),
	myOperator(myOperator),
	myLabel(label)
{
	myReg.push_back(reg1);
	myReg.push_back(reg2);
}
```
為用來建構一個Instruction包含一個Register和一個Label的Constructor

Ex: beq R5,R3,EndLoopJ

#### 2. getAddress
```C++
int getAddress()
{
	return myAddress;
}
```
回傳這個Instruction的address

### 3. exection
```c++
bool exection()
{
	if (myOperator == "li")
	{
		*myReg[0] = myImmediate;
	}
	else if (myOperator == "add")
	{
		*myReg[0] = *myReg[1] + *myReg[2];
	}
	else if (myOperator == "addi")
	{
		*myReg[0] = *myReg[1] + myImmediate;
	}
	else if (myOperator == "beq")
	{
		if (*myReg[0] == *myReg[1])
			return true;
	}
	else if (myOperator == "bne")
	{
		if (*myReg[0] != *myReg[1])
			return true;
	}
	else if (myOperator == "andi")
	{
		*myReg[0] = myReg[1]->getValue() & myImmediate;
	}
	return false;
}
```
判斷當myOperator為特定的Operator時，

執行相對應的運算，若是會branch則return true，

若是不會branch則return false

### 4. getLabel
```c++
string getLabel()
	{
		return myLabel;
	}
```
回傳myLabel


### Friend:
#### 1. ostream& operator<<(ostream& output, Instruction inst)
```C++
ostream& operator<<(ostream& output, Instruction inst)
{
	output << inst.myOperator << ' ' << 'R' << findRegIndex(inst.myReg[0], registers) << ',';
	if (inst.myReg.size() == 3)//ex:add R6,R5,R4
		output << 'R' << findRegIndex(inst.myReg[1], registers)
		<< ','  << 'R' << findRegIndex(inst.myReg[2], registers);
	else if (inst.myLabel.size() != 0)//ex:beq R5,R3,EndLoopJ 
		output << 'R' << findRegIndex(inst.myReg[1], registers) << ',' << inst.myLabel;
	else if (inst.myReg.size() == 2)//ex:addi R5,R5,1
		output  << 'R' << findRegIndex(inst.myReg[1], registers) << ',' << inst.myImmediate;
	else if (inst.myReg.size() == 1)//ex:li R3,16
		output << inst.myImmediate;
	return output;
}
```
先判斷Instruction的類型，在輸出其對應的格式

### B.Register
### Private
### 1. myValue
```c
int myValue;
```
用來記錄Register內的值
### Public
### 1. Constructor
```c
Register()
	:myValue()
{}
```
初始化myValue

### 2. operator=
```
int& operator=(int value)
{
	myValue = value;
	return myValue;
}
```
將傳入的型態為int的值傳給myValue

並回傳myValue

### 3. operator+
```c++
int& operator+(int value)
{
	int v = myValue + value;
	return v;
}
```
將傳入型態為int的值加上myValue，並回傳

```c++
int& operator+(Register r)
{
	int v = myValue + r.myValue;
	return v;
}
```
將傳入的Register其myValue加上自身的myValue，並回傳

### 4. operator==
```c
bool operator==(Register r)
{
	return (myValue == r.myValue);
}
```
如是傳入的Register的myValue等於自身的myValue則回傳true，反之亦然

### 5. operator!=
```c
bool operator!=(Register r)
{
	return (myValue != r.myValue);
}
```
如是傳入的Register的myValue不等於自身的myValue則回傳true，反之亦然

### 6. getValue
```c
int getValue()
{
	return myValue;
}
```
回傳myValue

### C. Label
### Private
### 1. name
```c
string name
```
用來儲存Label的名稱

### 2. address
```c
int address
```
用來儲存Label的位置

### Public
### 1. Constructor
```C++
Label(string n, int a)
	:name(n),
	address(a)
{}
```
將傳入的值做初始化

### 2. getAddress
```C
int getAddress()
{
	return address;
}
```
回傳Label的address

### 3. getName
```C
string getName()
{
	return name;
}
```
用來回傳Label的名稱

### C. Predictor
### Private
### 1. index
```C
int index
```
為當前Predictor所使用的2-bit counter

Ex: 01 則 index為2

### 2. historyBit
```C
vector <int> historyBit
```
為用來存放的3-bit history的值

Ex: 0 1

### 3. state
```C
vector <string> state
```
用來存放preditor的8個2-bit history counter

Ex: WN, SN, ST, SN, SN, SN, SN, SN

### 4. misprediction
```C
int misprediction;
```
用來存放misprediction的次數

### Public
### 1. Constructor
```C
Predictor():
	index(),
	misprediction()
{
	historyBit.resize(3);
	for (int i = 0; i < 3; i++)
		historyBit[i] = 0;
	state.resize(8);
	for (int i = 0; i < 8; i++)
		state[i] = "SN";
}
```
用來初始化index和misprediction，

且將historyBit的大小設定成3，

再將state的大小設定成8，

最後再將state內都先設定為SN

### 2. prediction
```C
bool prediction()
{
	index = 0;
	int count = 0;
	for (int i = 2; i >= 0; i--)
		index += pow(2, count++) * historyBit[i];
	if (state[index][1] == 'N')
		return false;
	else
		return true;
}
```
先透過history計算出計算出index，

再根據對應的state回傳true or false

### 3. update
```C
void update(bool myPred, bool outcome)
{
	historyBit[0] = historyBit[1];
	historyBit[1] = historyBit[2];
	historyBit[2] = outcome;
	if (outcome)//taken
	{
		if (state[index] == "SN")
			state[index] = "WN";
		else if (state[index] == "WN")
			state[index] = "WT";
		else if(state[index] == "WT")
			state[index] = "ST";
	}
	else//noTaken
	{
		if (state[index] == "WN")
			state[index] = "SN";
		else if (state[index] == "WT")
			state[index] = "WN";
		else if(state[index] == "ST")
			state[index] = "WT";
	}
	if(myPred!=outcome)
		misprediction++;		
}
```
先將historyBit向左一位且最右邊的一位為這次真正的結果，

再根據這次的結果來更新state，

且當prediction != outcome時misprediction+1

### 4. getMispred
```
int getMispred()
{
	return misprediction;
}
```
回傳misprediction
