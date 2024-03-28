#include <iostream>
#include <algorithm>
#include <ctime>


// dont end section with i/I
// dont start a section with a/A
// end every section with a punctuation mark (.?!,: OK, but no other punctuation/symbols currently supported)

using namespace std;

void Encrypt(string& entry); 

void removeApostrophes(string& str);
bool Modify(string& str, const string& from, const string& to);
void SoloI(string& data, string toSearch, string replaceStr); 
void Swap(string& word); 
void Encode1(string& word, string& firstLetters);
void Encode2(string& word, string& firstLetters, int& spaces, int check);
void Scramble(string& word); 
void CleanUpXs(string& str); 

int main()
{

	string entry = "Not everything needs some kind of keepsake to retain value.";
	Encrypt(entry);

	return 0; 
}




void Encrypt(string& entry)
{
	srand(time(NULL));

	//remove apostrophes
	removeApostrophes(entry); 

	//all x's are changed to ks, etc. 
	for(int i = 0; i <= entry.size(); i++)
	{
		Modify(entry, "x", "ks");
		Modify(entry, " a ", "a ");
		Modify(entry, " A ", "A ");
	}


	string encoded1;
	string word;
	char punc = ' ';
	bool firstWordI = false;
	bool firstWord = true;

	//step 1: switch first and last letter of each word 
	for (int i = 0; i <= entry.size(); i++) //iterate through the entire entry 
	{
		if (entry[i] == ' ' || i == entry.size())
		{
			if (firstWord == true)
			{
				if (word == "I" || word == "i")
				{
					firstWordI = true;
				}
				firstWord = false;

			}
			Swap(word);
			if (punc != ' ')
				encoded1 = encoded1 + word + punc + " ";
			else
				encoded1 = encoded1 + word + " ";
			word = "";
			punc = ' ';
		}
		else if (entry[i] == '?' || entry[i] == '.' || entry[i] == '!' || entry[i] == ',' || entry[i] == ':')
		{
			punc = entry[i];
		}
		else
		{
			word = word + entry[i];
		}
	}

	//step 1.5: make everything uppercase (except standalone i)
	transform(encoded1.begin(), encoded1.end(), encoded1.begin(), ::toupper);
	SoloI(encoded1, " I ", " i ");
	if (firstWordI == true)
	{
		encoded1[0] = 'i';
	}



	//step 2:
	//part 1 - remove first char of each word (and save it for later) 
	word = "";
	string encoded2; 
	string firstLetters = "";
	for (int i = 0; i <= encoded1.size(); i++)
	{
		if (encoded1[i] == ' ' || i == encoded1.size())
		{
			Encode1(word, firstLetters);
			encoded2 = encoded2 + word + " ";
			word = "";
		}
		else if (encoded1[i] != ' ')
		{
			word = word + encoded1[i];
		}
	}



	//part 2 - concatenate first char of prev word to end of next word (skip first word) 
	word = "";
	string encoded3; 
	int spaces = 0;

	for (int i = 0; i < encoded2.size(); i++)
	{
		if (encoded2[i] == ' ')
		{
			spaces++;
			if (spaces == 1)
			{
				if (word == "i") // we skip a cycle of appending if theres an i
				{
					spaces = 0;
					encoded3 = encoded3 + word + " ";
					word = "";
				}
				else if (word.find_first_of("?.!:,") != std::string::npos)
				{
					Encode2(word, firstLetters, spaces, 1);
					encoded3 = encoded3 + word + " ";
					word = "";
				}
				else
				{
					firstWordI = false;
					encoded3 = encoded3 + word + " ";
					word = "";
				}
			}
			else if (spaces != 1)
			{
				Encode2(word, firstLetters, spaces, 0);
				if (word.size() == 1 && word != "i")
				{
					const string punctuation_marks = "?.,!:";
					size_t pos = encoded3.length();

					while (pos > 0) {
						--pos;
						if (punctuation_marks.find(encoded3[pos]) != string::npos) {
							encoded3.insert(pos, " " + word);
							break;
						}
					}
				}
				else
				{
					encoded3 = encoded3 + word + " ";
					word = "";
				}
			}
		}

		else if (encoded2[i] != ' ')
		{
			word = word + encoded2[i];
		}
	}



	//step 3: scramble it up 
	word = "";
	string encoded4; 

	for (int i = 0; i <= encoded3.size(); i++)
	{
		if (encoded3[i] == ' ')
		{
			Scramble(word);
			encoded4 = encoded4 + word + " ";
			word = "";
		}
		else if (encoded3[i] != ' ')
		{
			word = word + encoded3[i];
		}
	}
	//remove any trailing spaces
	int len = encoded4.length();
	for (int i = len - 1; i >= 0; i--)
	{
		if (encoded4[i] == ' ')
		{
			encoded4.erase(i, 1);
		}
		else
		{
			break;
		}
	}

	//step 4: add X's
	word = "";
	spaces = 0;

	for (int i = 0; i <= encoded4.size(); i++)
	{
		if (encoded4[i] == ' ')
		{
			spaces++;
			if ((spaces % 2 != 0) && (encoded4[i - 1] != '?' && encoded4[i - 1] != '!' && encoded4[i - 1] != '.'
				&& encoded4[i - 1] != ',' && encoded4[i - 1] != ':'))
			{
				encoded4[i] = 'X';
			}
		}
		else if (encoded4[i] != ' ')
		{
			word = word + encoded4[i];
		}
	}

	//to make sure that every sentence ends with a single letter, no X's after or right before
	CleanUpXs(encoded4);

	std::cout << encoded4 << endl;
}

void removeApostrophes(string& str) {
	str.erase(remove(str.begin(), str.end(), '\''), str.end()); // ASCII apostrophe
	str.erase(remove(str.begin(), str.end(), '\u2018'), str.end()); // Opening single quote
	str.erase(remove(str.begin(), str.end(), '\u2019'), str.end()); // Closing single quote
}

bool Modify(string& str, const string& from, const string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void SoloI(string& data, string toSearch, string replaceStr)
{
	size_t pos = data.find(toSearch);

	while (pos != string::npos)
	{
		data.replace(pos, toSearch.size(), replaceStr);
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

void Swap(string& word)
{
	int first = 0;
	int last = word.size() - 1;

	char f = word[first];
	char l = word[last];

	word[first] = l;
	word[last] = f;
}

void Encode1(string& word, string& firstLetters)
{
	if (word == "i" || word == "I")
	{
		//do nothing 
	}
	else
	{
		firstLetters = firstLetters + word[0];
		word.erase(0, 1);
	}
}

void Encode2(string& word, string& firstLetters, int& spaces, int check)
{
	static int count = 0;

	if (check != 0) // one word sentence 
	{
		const std::string punctuation_marks = "?.,!:";
		size_t pos = word.find_first_of(punctuation_marks);
		if (pos != std::string::npos) {
			word.insert(pos, " ");
			word.insert(pos + 1, firstLetters.substr(count, 1));
		}
		spaces = 0;
		count++;
	}
	else
	{
		if (word == "i")
		{
			// do nothing 
		}
		else if (word.find('?') != std::string::npos || word.find('!') != std::string::npos || word.find('.') != std::string::npos
			|| word.find(',') != std::string::npos || word.find(':') != std::string::npos)
		{
			size_t pos = 0;

			if (word.find('?') != std::string::npos)
			{
				pos = word.find('?');
			}
			else if (word.find('!') != std::string::npos)
			{
				pos = word.find('!');
			}
			else if (word.find('.') != std::string::npos)
			{
				pos = word.find('.');
			}
			else if (word.find(',') != std::string::npos)
			{
				pos = word.find(',');
			}
			else if (word.find(':') != std::string::npos)
			{
				pos = word.find(':');
			}

			word.insert(pos, firstLetters.substr(count, 1) + " ");
			count++;
			word.insert(word.size() - 1, firstLetters.substr(count, 1));
			count++;
			spaces = 0;
		}
		else
		{
			word = word + firstLetters[count];
			count++;
		}
	}
}

void Scramble(string& word)
{

	string bank = "abdef";

	int posFor34;
	int posFor56;
	int posFor7Plus;

	char randomCharNum;
	char randomChar;

	if (word.size() <= 2)
	{
		//do nothing 
	}
	else if (word.size() >= 3 && word.size() <= 4)
	{
		for (int i = 0; i < 2; i++)
		{
			posFor34 = rand() % 3;
			randomCharNum = rand() % 5;
			randomChar = bank[randomCharNum];

			word.insert(posFor34, 1, randomChar);

		}

	}
	else if (word.size() >= 5 && word.size() <= 6)
	{
		for (int i = 0; i < 3; i++)
		{
			posFor56 = rand() % 5;
			randomCharNum = rand() % 5;
			randomChar = bank[randomCharNum];

			word.insert(posFor56, 1, randomChar);

		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			posFor7Plus = rand() % 7;
			randomCharNum = rand() % 5;
			randomChar = bank[randomCharNum];

			word.insert(posFor7Plus, 1, randomChar);
		}
	}
}

void CleanUpXs(string& str)
{
	for (std::size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '.' || str[i] == ',' || str[i] == '?'
			|| str[i] == '!' || str[i] == ':' || str[i] == ';')
		{
			std::size_t pos = i - 2;
			if (pos < str.size() && str[pos] == 'X')
			{
				str[pos] = ' ';
			}
		}
	}
}
