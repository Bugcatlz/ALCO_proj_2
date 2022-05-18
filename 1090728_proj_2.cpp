#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include "Instruction.h"
#include "Register.h"
#include "Label.h"
#include "Predictor.h"
using namespace std;


vector <Register*> registers(32); //共32個的register
void readfile(vector <Instruction>& insts, vector <Label>& labs);//讀檔
Instruction converInst(string input);//將讀檔後一整行的string轉為Instruction
string converLabel(string input);//將讀檔後一整行的string轉成Label中的string
int addressToIndex(int a, vector <Instruction>& insts);//當address為a時，其在instructions中的index => 第幾個instruction
int labelToAddress(string l, vector <Label>& labs);//當Label為l時，找其address
int stringToint(string s);//將16進位的string轉成int
int findRegIndex(Register* r, vector <Register*> registers);//當register的位置為r時，找到其在regiseters中的位置
bool isPowerByTwo(int num); // 用來判斷N是否為2的次方數
ostream& operator<<(ostream& output, Instruction inst);//Instruction的輸出
ostream& operator<<(ostream& output, Predictor pred);//Predictor的輸出
int main(void)
{
	for (int i = 0; i < 32; i++)//初始化32個pointer to register
		registers[i] = new Register;
	vector <Instruction> insts;//用來裝所有的instruction
	vector <Label> labs;//用來裝所有的label
	readfile(insts, labs);
	int num;//用來存放number of entry

	cout << "Please input entry(entry > 0):" << endl;
	cin >> num;

	if (num <= 0 || !isPowerByTwo(num) ) //大於零且為二的次方
	{
		cout << "Number of entries is an error!" << endl;
		system("pause");
		exit(1);
	}

	vector <Predictor> preds;//用來存放所有的Predictor
	preds.resize(num);
	int next = insts[0].getAddress();
	while (next != -1)//下一個不為最後一個label
	{
		//predictor
		int entry = (next / 4) % num;//找到為第幾的entry
		bool predBranch = preds[entry].prediction();//Prediction的結果
		//instruction
		int index = addressToIndex(next, insts);//為第幾個instruction
		bool outcome = insts[index].exection();//執行運算
		if (outcome)
		{
			string nextLabel = insts[index].getLabel();
			next = labelToAddress(nextLabel, labs);
		}
		else
			next = insts[index + 1].getAddress();
		cout << "entry: " << entry << "\t\t" << insts[index] << endl;
		cout << "prediction: ";
		if (predBranch)
			cout << "T";
		else
			cout << "N";
		cout << "\t\t";
		cout << "outcome: ";
		if (outcome)
			cout << "T";
		else
			cout << "N";
		cout << endl;
		
		for (int i = 0; i < preds.size(); i++)
			cout << i << "." << preds[i] << endl;	
		preds[entry].update(predBranch, outcome);
		cout << "misprediciton: " << preds[entry].getMispred() << endl;
		cout << "------------------------------------------" << endl;
		
	}
	system("pause");
}

void readfile(vector <Instruction>& insts, vector <Label>& labs)
{
	ifstream inFile("input.txt", ios::in);
	if (!inFile)
	{
		cout << "File could not be opened!!" << endl;
		exit(1);
	}
	cin.clear();
	string temp;
	vector <string> input;//用來存讀檔後的每行的string
	while (getline(inFile, temp))
		input.push_back(temp);
	int i = 0;
	while (i < input.size())
	{
		if (input[i][0] != '\t')//判斷是否不為label
			insts.push_back(converInst(input[i]));
		else
		{
			if (i != input.size() - 1)//判斷是否不為最後一個label
			{
				string n = converLabel(input[i]);//name
				Label temp(n, insts[insts.size() - 1].getAddress()+4);
				labs.push_back(temp);
			}
			else
			{
				string n = converLabel(input[i]);
				int a = -1;//address
				Label temp(n, a);//設定最後一個label的位置為-1
				labs.push_back(temp);
			}
		}
		i++;
	}
	inFile.close();
}

Instruction converInst(string input)
{
	string addressString;//用來存放型態為string的address
	string Operator;
	vector <string> regs;//用來存存放當中全部的register
	string reg; //用來存register的index ex:R8 中的 8
	string imm; //用來存immediate型態為string
	string label; 
	int Address; 
	bool flag = false; //是否為最後一個operand
	int k = 0;  //input的index
	//address
	while (input[k] != '\t')
		addressString.push_back(input[k++]);
	Address = stringToint(addressString);
	//address和operator之間的兩個\t
	k++;
	k++;
	//operator
	while (input[k] != ' ')
		Operator.push_back(input[k++]);
	k++;
	//第一個register
	k++;	//去除R
	while (input[k] != ',')
		reg.push_back(input[k++]);
	regs.push_back(reg);
	reg.clear();
	k++;
	//第二個
	if (input[k] == 'R')//為reg
	{
		k++;	//去除R
		while (input[k] != ',')
			reg.push_back(input[k++]);
		regs.push_back(reg);
		reg.clear();
		k++;
	}
	else
	{
		while (true)
		{
			imm.push_back(input[k++]);
			if (input[k] == '\t')//判斷是否為最後一個
			{
				flag = true;
				break;
			}
			if (input[k] == ',')
			{
				k++;
				break;
			}
		}
	}

	if (!flag)//不為最後一個
	{
		if (input[k] == 'R')//判斷是否為register
		{
			k++; //去除R
			while (!(input[k] == '\t' || input[k] == ' '))
				reg.push_back(input[k++]);
			regs.push_back(reg);
		}
		else if ((input[k] <= '9' && input[k] >= '0') || input[k] == '-')//判斷是否為immeditate
		{
			while (!(input[k] == '\t' || input[k] == ' '))
				imm.push_back(input[k++]);
		}
		else//為label
		{
			while (!(input[k] == '\t' || input[k] == ' '))
				label.push_back(input[k++]);
		}
	}

	if (regs.size() == 3)//ex:add R6,R5,R4
	{
		Instruction temp(Address, Operator, registers[stoi(regs[0])], registers[stoi(regs[1])], registers[stoi(regs[2])]);
		return temp;
	}
	if (label.size() != 0)//ex:beq R5,R3,EndLoopJ 
	{
		Instruction temp(Address, Operator, registers[stoi(regs[0])], registers[stoi(regs[1])], label);
		return temp;
	}
	if (regs.size() == 1)//ex:li R3,16
	{
		Instruction temp(Address, Operator, registers[stoi(regs[0])], stoi(imm));
		return temp;
	}
	if (regs.size() == 2)//ex:addi R5,R5,1
	{
		Instruction temp(Address, Operator, registers[stoi(regs[0])], registers[stoi(regs[1])], stoi(imm));
		return temp;
	}
}

string converLabel(string input)
{
	int k = 1; //去除第一個\t
	string temp;
	while (k != input.size())
		temp.push_back(input[k++]);
	return temp;
}


int addressToIndex(int a, vector <Instruction>& insts)
{
	for (int i = 0; i < insts.size(); i++)
	{
		if (a == insts[i].getAddress())
			return i;
	}
}

int labelToAddress(string l, vector <Label>& labs)
{
	for (int i = 0; i < labs.size(); i++)
	{
		string temp = labs[i].getName();
		if (temp == l)
			return labs[i].getAddress();
	}
}

int stringToint(string s)
{
	int sum = 0;
	int count = 0;
	int i = s.size() - 1;
	while (s[i] != 'x')
	{
		if(s[i]<='9' && s[i] >='0')
			sum += pow(16, count++) * (s[i]-'0');
		else
		{
			int temp = s[i] - 'A' + 10;
			sum += pow(16, count++) * temp;
		}
		i--;
	}
	return sum;
}

int findRegIndex(Register* r, vector <Register*> registers)
{
	for (int i = 0; i < registers.size(); i++)
		if (registers[i] == r)
			return i;
}

bool isPowerByTwo(int num)
{
	return num > 0 && (num & num - 1) == 0;
}

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

ostream& operator<<(ostream& output, Predictor pred)
{
	output << "(";
	for (int i = 0; i < 3; i++)
		output << pred.historyBit[i];
	for (int i = 0; i < 8; i++)
		output << ", " << pred.state[i];
	output << ")";
	return output;
}