#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Record {
    string word;
    int pages[101] = {};
    int count = 0;
};

int main() {
    //initialize all variables
    ifstream in("book.txt");
    string ban[] =
    { "the", "for", "in", "into", "are", "but", "is", "on", "a","an", "of", "to", "at", "by", "and", "not", "or" };
    int j = 0;
    int sizeban = sizeof(ban) / sizeof(string);
    //size
    int WORDS_IN_DICTIONARY = 5;
    int WORDS_NUMBER = 0;

    int count = 0;

    const int ROWS_ON_PAGE = 25;
    int PAGE_NUMBER = 0;
    int ROW_NUMBER = 0;
    int ROW_LENGTH = 0;
    int ROW_POINTER = 0;
    bool found = false;
    string row = "", word = "";
    int i = 0;
    string temp = "";
    Record* result = new Record[WORDS_IN_DICTIONARY];
readPage:
    if (in.peek() == EOF) {
        goto outerFor;
    }
    PAGE_NUMBER++;
    ROW_NUMBER = 0;
readRows:
    ROW_NUMBER++;
    if (ROW_NUMBER > ROWS_ON_PAGE || in.peek() == EOF) goto readPage; // if new page started
    getline(in, row);
    //reset row len to count it
    ROW_LENGTH = 0;
    if (row[0] == '\0') goto readRows; // if empty row
readCurrentRow:
    ROW_POINTER = 0;
countLength:
    if (row[ROW_LENGTH]) {
        ROW_LENGTH++;
        goto countLength;
    }

    word = "";
readWord:
    if (ROW_POINTER < ROW_LENGTH) {
        if (row[ROW_POINTER] == ' ' || row[ROW_POINTER] == '\0') {
            goto afterReadWord;
        }
        word += row[ROW_POINTER];
        ROW_POINTER++;
        goto readWord;
    }
    else {
        goto afterReadWord;
    }

afterReadWord:
    j = 0;
toLowerCase:
    if (word[j] != '\0') {
        if (word[j] <= 'Z' && word[j] >= 'A') {
            word[j] += 32;
        }
        j++;
        goto toLowerCase;
    }
    else {
    checkLast:
        if (
            !(word[j - 1] <= 'Z' && word[j - 1] >= 'A')
            &&
            !(word[j - 1] <= 'z' && word[j - 1] >= 'a')
            )
        {
            word[j - 1] = '\0';
            found = true;
            j--;
            goto checkLast;
        }
    }
    if (found) {
        temp = "";
        i = 0;
    thisFor:
        if (i >= j) {
            goto afterFor;
        }
        temp += word[i];
        i++;
        goto thisFor;
    afterFor:
        word = temp;
        j--;
        found = false;
        i = 0;
    }
checkBanWord:
    if (i >= sizeban) goto afterCheckBanWord;
    if (word == ban[i]) {
        ROW_POINTER++;
        word = "";
        goto readWord;
    }
    else {
        i++;
        goto checkBanWord;
    }
afterCheckBanWord:
    i = 0;
    count = 0;
searchDictionary:
    if (count >= WORDS_IN_DICTIONARY) {
        goto newWord;
    }
    else {
        if (result[count].word == word) {
            int k = result[count].count;
            if (k <= 100 && result[count].pages[k - 1] != PAGE_NUMBER) {
                result[count].pages[k] = PAGE_NUMBER;
                result[count].count++;
            }
            goto addingEnd;
        }
        else {
            count++;
            goto searchDictionary;
        }
    }

newWord:
    if (WORDS_NUMBER == WORDS_IN_DICTIONARY) {
        Record* result_copy = new Record[WORDS_IN_DICTIONARY * 2];
        int size = 0;
    copyWords:
        if (size >= WORDS_IN_DICTIONARY) {
            goto afterCopy;
        }
        result_copy[size] = result[size];
        size++;
        goto copyWords;
    afterCopy:
        WORDS_IN_DICTIONARY *= 2;
        result = new Record[WORDS_IN_DICTIONARY];

        size = 0;
    extendDick:
        if (size >= WORDS_IN_DICTIONARY) {
            goto addNewWord;
        }
        result[size] = result_copy[size];
        size++;
        goto extendDick;
    }
addNewWord:
    result[WORDS_NUMBER].count = 1;
    result[WORDS_NUMBER].pages[0] = PAGE_NUMBER;
    result[WORDS_NUMBER].word = word;
    goto addingEnd;


addingEnd:
    WORDS_NUMBER++;
    word = "";
    ROW_POINTER++;
    //cout << row << endl;
    if (ROW_POINTER > ROW_LENGTH) goto readRows;
    goto readWord;
outerFor:
    if (i < WORDS_IN_DICTIONARY - 1) {
        j = 0;
    innerFor:
        if (j < WORDS_IN_DICTIONARY - i - 1) {
            if (result[j].word > result[j + 1].word) {
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
finish:
    i = 0;
    
    ofstream out("result.txt");
output:
    if (i >= WORDS_IN_DICTIONARY) goto end;
    if (result[i].count < 101 && result[i].count > 0) {
        cout << result[i].word << " -> ";
        j = 0;
    print_page_numbers:
        if (j >= result[i].count) {
            goto endLine;
        }
        cout << result[i].pages[j] << ", ";

        j++;
        goto print_page_numbers;
    }

endLine:
    if (result[i].count < 101 && result[i].count > 0) {
        cout << endl;
    }
    i++;
    goto output;

end:
    in.close();
    cout << "Successfully finished program" << endl;
    return 0;
}