#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <thread> 
#include <ctime>
#include <cctype>
#include <map>
#include <mutex>
#include <list>
#include <concurrent_unordered_map.h>

using namespace concurrency;

using namespace std;

mutex mapMutex;

string formatStr(string& str)
{
	string newStr = "";
	for (char& ch : str)
	{
		if (!iswalpha((unsigned char)ch))
			continue;

		ch = tolower(ch);
		newStr.push_back(ch);
	}

	//for (int i = 0; i < str.size(); i++)
	//{
	//	if (!isalpha((unsigned char)str[i]))
	//		continue;

	//	char ch = tolower(str[i]);
	//	newStr.push_back(ch);

	//}
	return newStr;
}

vector<string>* formatFile(vector<string>* text)
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

vector<string>* readFileByWords(string fileName)
{
	vector <string>* vec = new vector < string >;
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

vector<string>* readStringByWord(string str)
{
	vector<string>* vec = new vector < string >;
	istringstream iss(str);
	string word;
	while (iss >> word)
	{
		vec->push_back(word);
	}

	return formatFile(vec);
}

void computeFrequencyNew(int n, vector <string>* text, int start, int end, concurrent_unordered_map<string, int>* resMap)
{
	if (!n || !text->size()) //Если хотя бы один вектор пуст
		return;

	string word;

	if (text->size() < n)
		return;

	int endOfEnd;
	if (end == text->size() - 1)
		endOfEnd = end - n+1;
	else
		endOfEnd = end;

	for (int i = start; i < endOfEnd; i++)
	{
		string str = "";
		for (int j = 0; j < n; j++)
		{
			str = str + " " + text->at(i + j);
		}

		concurrent_unordered_map<string, int>::iterator it;

		//mapMutex.lock();

		it = resMap->find(str);

		if (it == resMap->end())
			resMap->insert(pair<string, int>(str, 1));
		else
			it->second++;

		//mapMutex.unlock();
	}

	return;
}

concurrent_unordered_map<string, int>::iterator findeMaxValue(concurrent_unordered_map<string, int>* resMap)
{
	int currentMax = 0;
	concurrent_unordered_map<string, int>::iterator iter;
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
	std::cout << "Program was started" << endl;

	string fileName("C:\\Users\\lera\\Documents\\Visual Studio 2013\\Projects\\parallel\\ParallelSimple\\ParallelSimple\\Пикник на обочине.txt");//("text.txt"); //("текст.txt");//fileName(argv[0]);//
	int numberOfProcesses = 4;

	if (argc >= 3)
	{
		fileName = argv[1];//("text.txt"); ("текст.txt");//
		numberOfProcesses = atoi(argv[2]);
	}
	float frequency = 0;
	//int n = 0;

	vector<string> *vecText = readFileByWords(fileName);

	if (vecText == nullptr)
	{
		std::cout << "error in reading data" << endl;
		return 1;
	}

	vecText = formatFile(vecText);
	unsigned int timeAfterReading = clock();


	int grammSize = 3;
	int textSize = vecText->size();

	//Делим на процессы
	vector<thread*> vecThreads;
	concurrent_unordered_map<string, int>* resMap = new concurrent_unordered_map < string, int >;

	int step = textSize / numberOfProcesses; //Пример: 8-длина текста, 2- число проц. 8/2=4
	int start = 0;						//от 0
	int end = step - 1;					//до 3 и от 4 до 7
	
	textSize = vecText->size();

	step = textSize / numberOfProcesses; //Пример: 8-длина текста, 2- число проц. 8/2=4
	start = 0;						//от 0
	end = step - 1;					//до 3 и от 4 до 7

	for (int i = 1; i <= numberOfProcesses; ++i)
	{
		auto th = new std::thread(computeFrequencyNew, grammSize, vecText, start, end, resMap);
		vecThreads.push_back(th);


		start = end - grammSize + 1;

		//последний процесс захватывает все слова до конце
		if (i == numberOfProcesses)
			end = textSize - 1;
		else
			end = end + step;
	}

	for (auto &th : vecThreads)
	{
		th->join();
		delete th;
	}
	
	//}

	concurrent_unordered_map<string, int>::iterator itMax = findeMaxValue(resMap);

	int n = itMax->second; //Числитель - число появлений конкретной nGramm-ы

	int L = resMap->size(); //Знаменатель - число всех возможных nGramm


	frequency = ((float)n / (float)L) * 100;

	std::cout.setf(std::ios::fixed);
	std::cout << "nGramm: " << itMax->first << endl;
	std::cout << "Number of occurrences:" << n << endl;
	std::cout << "Number of all nGramm:" << L << endl;

	std::cout << "Frequency:" << frequency << endl;

	delete vecText;
	delete resMap;

	unsigned int timeEnd = clock();
	unsigned int workTime = (timeEnd - timeAfterReading);// / 1000; //в секундах
	cout << "runtime = " << timeEnd << endl; // время работы программы  
	cout << "runtime without reading = " << workTime << endl; // время работы программы  
	system("pause");

	ofstream fout("workTime.txt", ios_base::app); // открываем файл для добавления информации к концу файла

	if (!fout.is_open()) // если файл не открыт
		cout << "Файл не может быть открыт!\n"; // сообщить об этом

	fout << workTime << endl;
	fout.close();

	return 0;
}