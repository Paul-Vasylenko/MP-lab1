#include <iostream>
#include <fstream>
#include <string>
#include <locale> 
using namespace std;

struct Record {
    string text;
    int count;
};

int main()
{
    ifstream in("input.txt");
    const int number_of_words = 25;
    Record result[number_of_words];
    int cur_size = 0;
    string currentWord;
    string ban[] =
    { "the", "for", "in", "into", "are", "but", "is", "on", "a","an", "of", "to", "at", "by", "and", "not", "or" };
    int j = 0;
    int i = 0;
    int sizeban = sizeof(ban) / sizeof(string);
    bool found = false;
    string temp;
whileTrue:
    //read the word
    if (!(in >> currentWord)) {
        goto finish;
    }
    //to lowercase
    j = 0;
toLowerCase:
    if (currentWord[j] != '\0') {
        if (currentWord[j] <= 'Z' && currentWord[j] >= 'A') {
            currentWord[j] += 32;
        }
        j++;
        goto toLowerCase;
    }
    else {
    checkLast:
        if (
            !(currentWord[j - 1] <= 'Z' && currentWord[j - 1] >= 'A')
            &&
            !(currentWord[j - 1] <= 'z' && currentWord[j - 1] >= 'a')
            )
        {
            currentWord[j - 1] = '\0';
            found = true;
            j--;
            goto checkLast;
        }
    }
    //check for ban
    if (found) {
        temp = "";
        i = 0;
    thisFor:
        if (i >= j) {
            goto afterFor;
        }
        temp += currentWord[i];
        i++;
        goto thisFor;
    afterFor:
        currentWord = temp;
        j--;
        found = false;
        i = 0;
    }

checkBanWord:
    if (i >= sizeban) goto afterCheckBanWord;
    if (currentWord == ban[i]) {
        goto whileTrue;
    }
    else {
        i++;
        goto checkBanWord;
    }

afterCheckBanWord:
    i = 0;
    //add to array
findSimilar:
    temp = result[i].text;
    if (i > cur_size) goto afterFindSimilar;
    if (temp == currentWord) {
        result[i].count += 1;
        goto whileTrue;
    }
    else {
        i++;
        goto findSimilar;
    }
afterFindSimilar:
    result[cur_size++] = { currentWord, 1 };
    goto whileTrue;
finish:
    i = 0;
outerFor:
    if (i < cur_size - 1) {
    j = 0;
    innerFor:
    if (j < cur_size - i - 1) {
        if (result[j].count < result[j + 1].count) {
            Record temp = result[j];
            result[j] = result[j + 1];
            result[j + 1] = temp;
        }
        j++;
        goto innerFor;
        }

        i++;
        goto outerFor;
    }
    i = 0;
print:
    if (i < cur_size) {
        cout << result[i].text << " - " << result[i].count << endl;
        i++;
        goto print;
    }
    return 0;
}
