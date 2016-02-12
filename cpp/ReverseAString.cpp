// Reverse order of words in a string

#include <iostream>
#include <string.h>

void reverse(char *str, int start, int end1) {
	char swap;
	int end = end1;

	for (int i = start; i < end; ++i, --end) {
		swap = str[i];
		str[i] = str[end - 1];
		str[end - 1] = swap;
	}

	// std::cout << "reverse: " << std::string(str + start, end1 - start) << ", start = " << start << ", end = " << end1 << std::endl;
}

// Single space separated words
// No leading/trailing spaces
void reverseOrderOfWordsInAString(char *charStr) {
	std::cout << "input = " << charStr << std::endl;

	int wordStart = 0;

	int slen = strlen(charStr);

	int i = 0;
	for (i = 0; i < slen; ++i) {
		if (charStr[i] == ' ') {
			reverse(charStr, wordStart, i);
			wordStart = i + 1;
		}
		else if (i == (slen - 1)) {
			reverse(charStr, wordStart, i + 1);
		}
	}

	reverse(charStr, 0, i);

	std::cout << "output = " << charStr << std::endl;
}

int main(int argc, char **argv) {
	char str[100];

	strlcpy(str, "Hello, What is the time?", sizeof(str));
	reverseOrderOfWordsInAString(str);

	strlcpy(str, "Hello", sizeof(str));
	reverseOrderOfWordsInAString(str);

	strlcpy(str, "Say hello to a b", sizeof(str));
	reverseOrderOfWordsInAString(str);

	strlcpy(str, "Say   hello to   a b", sizeof(str));
	reverseOrderOfWordsInAString(str);

	return 0;
}

