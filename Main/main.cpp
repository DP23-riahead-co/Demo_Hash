#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
struct Word
{
    string word;
    string meaning;
};
int countWord(fstream& fi) {
    string x, word, y;
    getline(fi, x);
    int count(0);
    while (fi) {
        if (x.length() > 1) {
            getline(fi, y);
            stringstream ss(y);
            getline(ss, word, ' ');
            if (y[0] == 'U' && word == "Usage") {
                getline(ss, word, '\n');
                getline(fi, x);
            }
            else if (y != "") {
                count++;
            }
        }
        else {
            getline(fi, x);
        }
    }
    return count+1;
}

int hashFunc(string s, int sizeTab)
{
    long sum = 0, mul = 1;
    for (int i = 0; i < s.length(); i++)
    {
        mul = (i % 4 == 0) ? 1 : mul * 256;
        sum += s.at(i) * mul;
    }
    return (int)(abs(sum) % sizeTab);
}
void insert(string word, string meaning, Word* hashTable, int hashTableSize)
{
    int index = hashFunc(word, hashTableSize);
    int h = 1;
    while (hashTable[index].word != "")
    {
        index = (index + h * h) % hashTableSize;
        h++;
    }
    hashTable[index].word = word;
    hashTable[index].meaning = meaning;
}
void readFile(fstream& fi, Word* hashTable, Word* wordList, int len) {
    fi.clear();
    fi.seekg(0);
    string text;
    getline(fi, text);
    int i(0);
    bool isUsage(false);
    while (fi && i < len) {
        if (text.length() <= 1 || text[1] == ' ') {
            getline(fi, text);
        }
        if (text.length() > 1) {
            int j = 0;
            string word = "";
            while (text[j] != ' ') {
                word = word + text[j];
                j++;
            }
            if (word == "Usage" && wordList[i - 1].word[0] == 'U') {
                isUsage = true;
            }
            else {
                wordList[i].word = word;
            }
            while (text[j] == ' ') {
                j++;
            }
            if (isUsage) {
                while (j < text.length()) {
                    wordList[i - 1].meaning = wordList[i - 1].meaning + text[j];
                    j++;
                }
            }
            else {
                while (j < text.length()) {
                    wordList[i].meaning = wordList[i].meaning + text[j];
                    j++;
                }
            }
            insert(wordList[i].word, wordList[i].meaning, hashTable, len);
            i++;
            getline(fi, text);
        }
        isUsage = false;
    };

}
void updateHashTable(Word*& hashTable, int& hashTableSize, int numWord) {
    int newHashTableSize = hashTableSize + numWord;
    Word* newHashTable = new Word[newHashTableSize];
    for (int i = 0; i < hashTableSize; i++)
    {
        insert(hashTable[i].word, hashTable[i].meaning, newHashTable, newHashTableSize);
    }
    hashTableSize = newHashTableSize;
    hashTable = newHashTable;
}
void addANewWord(Word* &hashTable, int& hashTableSize) {
    int numWord;
    string word, meaning;
    cout << "Input the number of word you want to add : ";
    cin >> numWord;
    cin.ignore();
    updateHashTable(hashTable, hashTableSize, numWord);
    for (int i = 0; i < numWord; i++) {
        cout << "Input word : ";
        getline(cin, word);
        cout << endl;
        cout << "Input meaning : ";
        getline(cin, meaning);
        insert(word, meaning, hashTable, hashTableSize);
    }
}
void search(string word, Word* hashTable, int hashTableSize)
{
    //Compute the index using the Hash Function
    int index = hashFunc(word, hashTableSize);
    //Search for an unused slot and if the index will exceed the hashTableSize roll back
    int h = 1;
    while (hashTable[index].word != word && hashTable[index].word != "")
    {
        index = (index + h * h) % hashTableSize;
        h++;
    }
    //Is the element present in the hash table
    if (hashTable[index].word == word)
    {
        cout << word << " is found!" << endl;
        cout << "Meaning : " << hashTable[index].meaning;
    }
    else
        cout << word << " is not found!" << endl;
}
void deleteWord(string word, Word* hashTable, int hashTableSize) {
    int index = hashFunc(word, hashTableSize);
    //Search for an unused slot and if the index will exceed the hashTableSize roll back
    int h = 1;
    while (hashTable[index].word != word && hashTable[index].word != "")
    {
        index = (index + h * h) % hashTableSize;
        h++;
    }
    //Is the element present in the hash table
    if (hashTable[index].word == word)
    {
        cout << word << " has been delete!" << endl;
        hashTable[index].word = "";
        hashTable[index].meaning = "";
    }
    else
        cout << word << " is not found!" << endl;
}
void editMeanning(string word, Word* hashTable, int hashTableSize) {
    int index = hashFunc(word, hashTableSize);
    //Search for an unused slot and if the index will exceed the hashTableSize roll back
    int h = 1;
    while (hashTable[index].word != word && hashTable[index].word != "")
    {
        index = (index + h * h) % hashTableSize;
        h++;
    }
    //Is the element present in the hash table
    if (hashTable[index].word == word)
    {
        string meaning;
        cout << "Input meaning of " << word << ":" << endl;
        getline(cin, meaning);
        hashTable[index].meaning = meaning;
    }
    else
        cout << word << " is not found!" << endl;
}
int main()
{
    fstream fi;
    string word, meaning;
    fi.open("Oxford English Dictionary.txt", ios::in);
   /* fi.open("test.txt", ios::in);*/
    int hashTableSize = countWord(fi);
    cout << hashTableSize;
    Word* hashTable = new Word[hashTableSize];
    Word* wordList = new Word[hashTableSize];
    readFile(fi, hashTable, wordList, hashTableSize);
    cout << "Input name to search : " << endl;
    getline(cin, word);
    search(word, hashTable, hashTableSize);
    return 0;
}