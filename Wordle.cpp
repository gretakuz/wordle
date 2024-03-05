#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <clocale>
#include <codecvt>
#include <cwchar>
#include <fcntl.h>

using namespace std;

bool isValidGuess(const wstring& guess) {
    return guess.length() == 5;
}

bool isIdentical(const wstring& word, const wstring& guess) {
    return word == guess;
}

int countCorrect(const wstring& word, const wstring& guess) {
    int count = 0;
    for (int i = 0; i < 5; ++i) {
        if (word[i] == guess[i])
            count++;
    }
    return count;
}

int countCorrectLetters(const wstring& word, const wstring& guess) {
    int count = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (word[i] == guess[j]) {
                count++;
                break;
            }
        }
    }
    return count;
}


int main() {
    // https://stackoverflow.com/questions/63922672/trouble-with-utf-8-input
    _setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stdin), _O_WTEXT);

    wifstream inputFile("zodynas5raides.txt");
    inputFile.imbue(std::locale(inputFile.getloc(), new std::codecvt_utf8<wchar_t>));
    if (!inputFile) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    vector<wstring> words;
    wstring word;

    while (getline(inputFile, word)) {
        words.push_back(word);
    }
    inputFile.close();

    if (words.empty()) {
        cerr << L"Error: No words found in the input file." << endl;
        return 1;
    }

    srand(time(0));
    wstring chosenWord = words[rand() % words.size()];

    wstring guess;
    int attempts = 0;

    wcout << L"Welcome to Wordle! Guess the 5-letter word within 6 attempts." << endl;

    while (attempts < 6) {
        wcout << L"Attempt #" << attempts + 1 << ": " << endl;

		getline(wcin, guess);
        wcin.ignore();

        if (!isValidGuess(guess)) {
            wcout << L"Please enter a valid 5-letter word." << endl;
            continue;
        }

        if (isIdentical(chosenWord, guess)) {
            wcout << L"Congratulations! You guessed the word: " << chosenWord << endl;
            break;
        } else {
            int correctLetters = countCorrect(chosenWord, guess);
            int correctPositions = countCorrectLetters(chosenWord, guess);

            wcout << L"Feedback:" << endl;
            for (int i = 0; i < 5; ++i) {
                wchar_t guessLetter = guess[i];
                if (chosenWord[i] == guessLetter) {
                    wcout << guessLetter << L" - Correct letter in correct position." << endl;
                } else {
                    bool found = false;
                    for (int j = 0; j < 5; ++j) {
                        if (chosenWord[j] == guessLetter) {
                            wcout << guessLetter << L" - Correct letter in wrong position." << endl;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        wcout << guessLetter << L" - Incorrect letter." << endl;
                    }
                }
            }

            wcout << L"Correct letters in correct positions: " << correctLetters << endl;
            wcout << L"Correct letters in wrong positions: " << correctPositions << endl;
        }

        attempts++;
    }

    if (attempts == 6) {
        wcout << L"Sorry, you've run out of attempts. The word was: " << chosenWord << endl;
    }

    return 0;
}
