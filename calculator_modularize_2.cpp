#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct token{
	string type;
	float number;
};

struct tokens{
	token cont;
	int index;
};

float calcu(string line);

token readNumber(string line,int &index)
{
	token ret;
	float number = 0;
	int flag = 0;		//小数点がでてきたら旗を上げる
	float keta = 0.1;
	//数字ではなくなるまで読む
	while(index < line.length())
	{
		if(isdigit(line[index]) || line[index] == '.'){
		//読み込んだものが小数点だったら旗を立てる
			if(line[index] == '.'){
				flag = 1;
			}else if(!flag){
				number = number * 10 + (line[index] - '0');
			}else{
				number += (line[index] - '0') * keta;
				keta *= 0.1;
			}
		}else{
			break;
		}
		index++;
	}
	ret.type = "NUMBER";
	ret.number = number;
	return ret;
} 

token readPlus(string line,int &index)
{
	token ret;
	ret.type = "PLUS";
	index++;
	return ret;
}

token readMinus(string &line,int &index)
{
	token ret;
	if (line[index+1] == '-'){
		ret.type = "PLUS";
		line[index+1] = '+';
	}else{
	ret.type = "MINUS";
	}
	index++;
	return ret;
}

token multi(string line,int &index,vector<tokens> &tokens0,const char &c)
{
	token ret,next;
	tokens last;
	ret.type = "NUMBER";
	index++;
	last = tokens0.back();		//一つ前の要素
	if (last.cont.type != "NUMBER")	//記号が二つ続いてたら
	{
		cout << "symbol error" << endl;
	}
	next = readNumber(line,index);
	if (c == '*')
	{
		ret.number = last.cont.number * next.number;
	}else if (c == '/')
	{
		ret.number = last.cont.number / next.number;
	}
	tokens0.pop_back();
	return ret;
}

void tokenize(string line,vector<tokens> &tokens0)
{
	int index = 0;
	int index2 = line.length()-1;
	tokens tokens1;
	//tokenにわけてtokensにいれる
	while(index < line.length()){
		if (isdigit(line[index])){
			tokens1.cont = readNumber(line,index);
			tokens1.index = index;
		}else if (line[index] == '+'){
			tokens1.cont = readPlus(line,index);
			tokens1.index = index;
		}else if (line[index] == '-'){
			tokens1.cont = readMinus(line,index);
			tokens1.index = index;
		}else if(line[index] == '*'){
			tokens1.cont = multi(line,index,tokens0,'*');
			tokens1.index = index;
		}else if (line[index] == '/'){
			tokens1.cont = multi(line,index,tokens0,'/');
			tokens1.index = index;
		}else{
			cout << "Invalid character found: " << line[index] << endl;
			cout << line[index] << endl;
			exit(1);
		}
		//配列に加える！
		tokens0.push_back(tokens1);
	}
}

float evaluate(vector<tokens> &tokens1)
{
	float answer = 0;
	//配列の最初に+をいれる！
	tokens tokens0;
	tokens0.cont.type = "PLUS";		//最初は必ず+!!
	tokens1.insert(tokens1.begin(),tokens0);
	//配列[0]には上で値を入れたからindexは1から
	int index = 1;

	while(index < tokens1.size()){
		if(tokens1[index].cont.type == "NUMBER"){
			if (tokens1[index-1].cont.type == "PLUS"){
				answer += tokens1[index].cont.number;
			}else if (tokens1[index-1].cont.type == "MINUS"){
				answer -= tokens1[index].cont.number;
			}else{

				cout << "Invalid syntax" << endl;
			}
		}
		index++;
	}
	return answer;
}

void check(string &line){
	int index =0;
	int index2 = line.length()-1;
	int flag =0;
	float newline;

	for (;index<index2;index++){
		if(line[index] == '('){
			for (; index2 >=index ; index2--){
				if(line[index2] == ')'){
					flag = 1;
					newline = calcu(line.substr(index+1,index2-index-1));
					line.replace(index,index2-index+1,to_string(newline));
					break;
				}
			}
			if (index<0){
			cout << "error" << endl;
			exit(1);
			}
		}
		if(flag){
			break;
		}
	}
}

float calcu(string line){
	std::vector<tokens> tokens0;
	float answer;

	check(line);
	tokenize(line,tokens0);
	answer = evaluate(tokens0);
	return answer;
}

int main(int argc, char const *argv[])
{
	string line = "";
	float answer = 0;

	cout << ">";
	cin >> line;
	answer = calcu(line);
	cout << "ansewr = " << answer << endl;
	return 0;
}