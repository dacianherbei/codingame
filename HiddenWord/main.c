#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct dictionaryNodeT {
	char* word;
	char marked;
	struct dictionaryNodeT* next;
};

struct dictionaryT {
	struct dictionaryNodeT* start;
	struct dictionaryNodeT* end;
};

struct dictionaryT* dictionary = (struct dictionaryT*) NULL;
char* wordTable;
char* marker;
int h = 0;
int w = 0;

char process(struct dictionaryNodeT* string, int line, int column,
		int heightPass, int widthPass) {
	int length = strlen(string->word);
	int columnIt = column;
	int lineIt = line;

	for (int j = 0; j < length;) {
		if (wordTable[lineIt * w + columnIt] != string->word[j]) {
			return 0;
		}
		columnIt += widthPass;
		lineIt += heightPass;
		j++;
		if (j == length) {
			string->marked = 1;
			columnIt = column;
			lineIt = line;
			for (int k = 0; k < length; k++) {
				marker[lineIt * w + columnIt] = 1;
				columnIt += widthPass;
				lineIt += heightPass;
			}
			return 1;
		}
		if (columnIt < 0 || lineIt < 0 || columnIt == w || lineIt == h) {
			return 0;
		}
	}

	return 0;
}

void processFromPosition(int line, int column) {
	char allRezult = 0;
	int index = line * w + column;
	for (struct dictionaryNodeT* it = dictionary->start; it != 0;) {
		char finalRezult = 0;

		for (int directionX = -1; directionX < 2; directionX++) {
			for (int directionY = -1; directionY < 2; directionY++) {
				finalRezult = process(it, line, column, directionX, directionY)
						| finalRezult;
				marker[index] = 0;
			}
		}
		if (finalRezult != 0) {
			it->marked = 1;
		}
		it = it->next;
		allRezult = allRezult | finalRezult;
	}
	marker[index] = allRezult;
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main() {
	int n;
	scanf("%d", &n);
	dictionary = malloc(sizeof(struct dictionaryT));
	for (int i = 0; i < n; i++) {
		char aword[41];
		scanf("%s", aword);
		struct dictionaryNodeT* newNode = malloc(
				sizeof(struct dictionaryNodeT));
		newNode->next = NULL;
		newNode->word = malloc(sizeof(char) * strlen(aword) + 1);
		strcpy(newNode->word, aword);

		if (dictionary->start == NULL) {
			dictionary->start = newNode;
			dictionary->end = newNode;
		} else {
			dictionary->end->next = newNode;
			dictionary->end = newNode;
		}
	}

	scanf("%d%d", &h, &w);
	wordTable = malloc(sizeof(char) * h * w);
	marker = malloc(sizeof(char) * h * w);
	for (int i = 0; i < h; i++) {
		char line[41];
		scanf("%s", line);
		strcat(wordTable + w * i, line);
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (marker[i * w + j] == 0) {
				processFromPosition(i, j);
			}
		}
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (marker[i * w + j] == 0) {
				printf("%c", wordTable[i * w + j]);
			}
		}
	}

	// cleanup
	for (struct dictionaryNodeT* it = dictionary->start; it != NULL;) {
		struct dictionaryNodeT* temp = it;
		free(it->word);
		it = temp->next;
		free(temp);
	}
	free(dictionary);
	free(wordTable);
	free(marker);

	return 0;
}
