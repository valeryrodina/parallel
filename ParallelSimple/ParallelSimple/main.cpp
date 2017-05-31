#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <unordered_map>

using namespace std;

string formatStr(string& str) //функция удаляет все лишние символы из слова и оставляет только буквы.
{
	string newStr = "";
	for (char& ch : str)
	{
		if (!iswalpha((unsigned char)ch))
			continue;

		ch = tolower(ch);
		newStr.push_back(ch);
	}
	return newStr;
}

vector<string>* formatFile(vector<string>* text) //функция удаляет все лишние символы из вектора слов и оставляет только буквы.
{
	vector<string>* newVec = new vector<string>;
	for (string& str : *text)
	{
		str = formatStr(str);
		if (strcmp(str.c_str(), "") != 0)
			newVec->push_back(str);
	}
	delete text;

	return newVec;
}

vector<string>* readFileByWords(string fileName) //функция позволяет считать файл и поместить его в вектор слов
{
	vector <string>* vec = new vector < string > ;
	ifstream file(fileName);
	
	if (!file.is_open())
		return nullptr;

	string word;

	while (file >> word)
	{
		vec->push_back(word);
	}

	return vec;
}

unordered_map<string, int>* computeFrequencyNew(int n, vector <string>* text) //функция посчитывает количество появлений n-граммы в тексте.
{
	if (!n || !text->size()) //Если хотя бы один вектор пуст
		return nullptr;

	string word;
	//int n = nGramm->size(); //n-gramm

	if (text->size() < n)
		return 0;


	unordered_map<string, int>* resMap = new unordered_map< string, int >;

	for (int i = 0; i < text->size()-n; i++)
	{
		string str = "";
		for (int j = 0; j < n; j++)
		{
			str = str + " " + text->at(i + j);
		}
		
		unordered_map<string, int>::iterator it;
		it = resMap->find(str);

		if (it == resMap->end())
			resMap->insert(pair<string, int>(str, 1));
		else
			it->second++;
	}

	return resMap;
}

unordered_map<string, int>::iterator findeMaxValue(unordered_map<string, int>* resMap)
{
	int currentMax = 0;
	unordered_map<string, int>::iterator iter;
	for (auto it = resMap->begin(); it != resMap->end(); ++it)
	{
		if (it->second > currentMax) 
		{
			currentMax = it->second;
			iter = it;
		}
	}
	return iter;
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "rus");

	cout << "Program was started" << endl;

	string fileName("C:\\Users\\lera\\Documents\\Visual Studio 2013\\Projects\\parallel\\ParallelSimple\\ParallelSimple\\Пикник на обочине.txt");//fileName(argv[0]);//("text.txt"); ("текст.txt");//
	
	if (argc >= 2)
	{
		fileName = argv[1];//("text.txt"); ("текст.txt");//
	}


	vector<string> *vecText = readFileByWords(fileName);
	if (vecText == nullptr)
	{
		cout << "File not found\n" << endl;
		delete vecText;
		return 1;
	}

	vecText = formatFile(vecText);
	unsigned int timeAfterReading = clock();


	unordered_map<string, int>* resMap = computeFrequencyNew(3, vecText);
	unordered_map<string, int>::iterator itMax = findeMaxValue(resMap);

	int n = itMax->second; //Числитель - число появлений конкретной nGramm-ы

	int L = resMap->size(); //Знаменатель - число всех возможных nGramm

	float frequency = ((float)n / (float)L) * 100;

	cout.setf(std::ios::fixed);
	cout << "nGramm: " << itMax->first << endl;
	cout << "Number of occurrences:" << n << endl;
	cout << "Number of all nGramm:" << L << endl;

	cout << "Frequency:" << frequency << "%" << endl;

	cout.precision(2); //2 - число символов после точки

	delete vecText;
	delete resMap;

	unsigned int timeEnd = clock();
	unsigned int workTime = (timeEnd - timeAfterReading);// / 1000; //в секундах
	cout << "runtime = " << timeEnd  << endl; // время работы программы  
	cout << "runtime without reading = " << workTime << endl; // время работы программы  
	system("pause");

	ofstream fout("workTime.txt", ios_base::app); // открываем файл для добавления информации к концу файла

	if (!fout.is_open()) // если файл не открыт
		cout << "Файл не может быть открыт!\n"; // сообщить об этом

	fout << workTime << endl;
	fout.close();
	return 0;
}