#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct node {
    int letter = -1;
    map<int, struct node*> next;
    bool marked = false;
};

struct tableItem {
    char letter;
    bool crossed = false;
};

static node root;
static vector<vector<struct tableItem>> wordTable;
static int h = 0;
static int w = 0;

template<int heightPass, int widthPass>
bool process(struct node& sequence, int line, int column, bool foundAll) {
    if (sequence.next.find(-1)!=sequence.next.end() ) {
        if ( !sequence.next[-1]->marked && !foundAll ) {
			sequence.next[-1]->marked = true;
			return (true);
        }
    }
    if (line == h || line < 0 || column == w || column < 0) {
		return (false);
    }
    char letter = wordTable[line][column].letter;
    if (sequence.next.find(letter) != sequence.next.end() ) {
        struct node* nextNode = sequence.next[letter];
        int nextLine = line+heightPass;
        int nextColumn = column+widthPass;
        if (process<heightPass,widthPass>(*nextNode,nextLine,nextColumn,sequence.marked && foundAll)) {
            wordTable[line][column].crossed = true;
            nextNode->marked = true;
			return (true);
        }
    }
	return (false);
}

/**
 * returns false when part of word
 *
 * processes only down-left, down, down-right and right
 * true otherwise
 */
void processFromPosition( int line, int column) {
    bool finalRezult = false;
    bool rezult = process<1,-1>(root,line,column, false);
    finalRezult = rezult;
    wordTable[line][column].crossed = false;

    rezult = process<1,0>(root,line,column, false);
    finalRezult = finalRezult || rezult;
    wordTable[line][column].crossed = false;

    rezult = process<1,1>(root,line,column, false);
    finalRezult = finalRezult || rezult;
    wordTable[line][column].crossed = false;


    rezult = process<-1,-1>(root,line,column, false);
    finalRezult = finalRezult || rezult;
    wordTable[line][column].crossed = false;


    rezult = process<-1,0>(root,line,column, false);
    finalRezult = finalRezult || rezult;
    wordTable[line][column].crossed = false;


    rezult = process<0,-1>(root,line,column, false);
    finalRezult = finalRezult || rezult;
    wordTable[line][column].crossed = false;


    rezult = process<-1,1>(root,line,column, false);
    finalRezult = finalRezult || rezult;
    wordTable[line][column].crossed = false;

    rezult = process<0,1>(root,line,column, false);
    finalRezult = finalRezult || rezult;
    wordTable[line][column].crossed = finalRezult;
}

void pushWord(string& word, node& forward, int index) {
    if (index < word.length()) {
        node* forward_next;
        if (forward.next.find(word[index])==forward.next.end()) {
            forward_next = new node();
            forward_next->letter = word[ index ];
            forward.next[word[index]] = forward_next;
        } else {
            forward_next = forward.next[word[index]];
        }

        index++;
        pushWord( word, *forward_next, index );
    } else {
        node* endNode = new node();
        forward.next[endNode->letter] = endNode;
    }
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int n;
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        string aword;
        cin >> aword; cin.ignore();
        pushWord(aword, root, 0);
    }
    cin >> h >> w; cin.ignore();
    wordTable.resize(h);
    for (int i = 0; i < h; i++) {
        string line;
        cin >> line; cin.ignore();
        wordTable[i].resize(w);
        for (int j = 0; j < w; j++) {
            wordTable[i][j].letter = line[j];
        }
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (!wordTable[i][j].crossed) {
                processFromPosition( i, j);
            }
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (!wordTable[i][j].crossed) {
                cout << wordTable[i][j].letter;
            }
        }
    }
}
