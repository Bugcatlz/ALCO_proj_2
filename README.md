ALO_proj_2
=== 
## Global Variable
### a. registers
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
		output << 'R' << findRegIndex(inst.myReg[1], registers) << ','  << 'R' << findRegIndex(inst.myReg[2], registers);
	else if (inst.myLabel.size() != 0)//ex:beq R5,R3,EndLoopJ 
		output << 'R' << findRegIndex(inst.myReg[1], registers) << ',' << inst.myLabel;
	else if (inst.myReg.size() == 2)//ex:addi R5,R5,1
		output  << 'R' << findRegIndex(inst.myReg[1], registers) << ',' << inst.myImmediate;
	else if (inst.myReg.size() == 1)//ex:li R3,16
		output << inst.myImmediate;
	return output;
}
```
