#include <iostream>
#include <fstream>
#include <string>

using namespace std;
struct MyStor {//for save all inform
    string nameOfFile;
    int sentenceCounter = 0;//порядковый номер предложения
    string commandLine;//line with command   6
    int pos = 0;// P2 position
    int blockerP2 = 0;
    int blockerH2 = 0;
    int blockerC7 = 0;
    int blockerP6 = 0;
    char *newWord = new char[CHAR_MAX];// P2 word
    char *word = new char[CHAR_MAX];//C7
    int wLength = 0;//c7 word length
};

MyStor myStor;


class Sentence//новое предложение
{
public:

    char **sentenceWith = new char *[CHAR_MAX];// указатель на начало слов и знаков пунктуации
    int sWCounter = 0;// подсчет количества элементов в sentenceWith
    char **punctuation = new char *[CHAR_MAX];// указатель на знаки пунктуации
    int *wordLength = new int[INT16_MAX];// для sentenceWith
    int nwLength = 0;// р2 newWord length
    int pCounter = 0;// подсчет количества элементов в punctuation
    int startOnThisWordNumber = 0; //h2 result

    //для создания нового предложения просто ввожу строку
    Sentence(string line) { splitter(line); }

    //метод парсит предложение на слова и вызывает метод добавления
    void splitter(string line) {
        int totalWordCount = 0;// количество букв в слове, на один меньше для записи в массив
        char *wordFast = new char[totalWordCount];
        //int flag = 0; //первое слово
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == ' ') {
                add(wordFast, totalWordCount);
                for (int j = 0; j < totalWordCount; ++j) {
                }
                totalWordCount = 0;

            } else {
                wordFast[totalWordCount] = line[i];
                totalWordCount++;
            }
        }
        add(wordFast, totalWordCount);
        commandParser();
        printer();

        delete[] wordFast;
    }

    // метод добавляет новое слово и заводит указатель на его начало в sentenceWith, punctuation, clearSentence
    void add(char *wordAdd, int length) {
        int fastLength = length;
        //если последняя буква в слове - знак препинания, делаю длину слова меньше на 1
        if (punctuationChecker(wordAdd[length - 1]))
            length -= 1;
        else if (wordAdd[length - 1] == '.') {
            if (wordAdd[length - 2] == '.')// проверяю на многоточие
                length -= 3;
            else length -= 1;
        }
        char *nWord = new char[length];
        for (int j = 0; j < length; ++j) {
            nWord[j] = wordAdd[j];
        }
        // указатель на начало слова завожу в необходимые массивы
        sentenceWith[sWCounter] = nWord;// в массив указателей вводим новый указатель на слово
        wordLength[sWCounter] = length;
        sWCounter++;
        length++;
        if (length == fastLength) //проверяю, был ли в конце знак препинания не равный многоточию
        {
            char *punct = new char[1];
            punct[0] = wordAdd[length - 1];
            sentenceWith[sWCounter] = punct;
            wordLength[sWCounter] = 1;
            sWCounter++;
            punctuation[pCounter] = punct;
            pCounter++;
        } else {
            length += 2;
            if (length == fastLength) {
                char *punct = new char[3] {'.','.','.'};
                sentenceWith[sWCounter] = punct;
                wordLength[sWCounter] = 3;
                sWCounter++;
                punctuation[pCounter] = punct;
                pCounter++;
            }
        }
    }

    // check punctuation mark
    // if the char is a punctuation mark function return true, and return false if is not
    static bool punctuationChecker(char mark) {
        switch (mark) {
            case ',':
                return true;
            case ':':
                return true;
            case ';':
                return true;
            case '-':
                return true;
            case '=':
                return true;
            case '_':
                return true;
            case '*':
                return true;
            case '(':
                return true;
            case ')':
                return true;
            case '{':
                return true;
            case '}':
                return true;
            case '[':
                return true;
            case ']':
                return true;
            case '/':
                return true;
            case '|':
                return true;
            case '\\':
                return true;
            case '+':
                return true;
            case '@':
                return true;
            case '#':
                return true;
            case '$':
                return true;
            case '%':
                return true;
            case '^':
                return true;
            case '!':
                return true;//возможно вынести в отдельную проверку
            case '?':
                return true;//возможно вынести в отдельную проверку
            default:
                return false;
        }
    }

// в этом методе буду разбивать строку комманд и сразу вызывать связанные методы
    void commandParser() {
        for (int i = 0; i < myStor.commandLine.length(); ++i) {
            int length = 0;
            int tempI;
            if (myStor.commandLine[i] == 'H' && myStor.blockerH2 == 0) {
                tempI = i;
                i += 4;//+ 2 для пропуска номера и точки
                while (myStor.commandLine[i] != ',') {
                    length++;
                    i++;
                }
                i = tempI;
                i +=4;
                char *nWord = new char[length];
                length = 0;
                while (myStor.commandLine[i] != ',') {
                    nWord[length] = myStor.commandLine[i];
                    length++;
                    i++;
                }
                startOnThisWord(nWord, length);
                delete[] nWord;
            }
            if (myStor.commandLine[i] == 'C' && myStor.blockerC7 == 0) {
                tempI = i;
                i += 4;//
                while (myStor.commandLine[i] != ',') {
                    length++;
                    i++;
                }
                i = tempI;
                i += 4;
                char *nWord = new char[length];
                length = 0;
                while (myStor.commandLine[i] != ',') {
                    nWord[length] = myStor.commandLine[i];
                    length++;
                    i++;
                }
                usingSumbolSequence(nWord, length);
                nwLength = length;
            }
            if (myStor.commandLine[i] == 'P') {
                int index = (int) myStor.commandLine[i + 4] - 48;
                if (index == myStor.sentenceCounter) {
                    if (myStor.commandLine[i + 2] == '6' && myStor.blockerP6 == 0) {
                        i += 6;
                        if (myStor.commandLine[i] == '0') {
                            i += 1;
                            deletePunctuationMark(0, reinterpret_cast<char *>('r'), 0);
                        } else {
                            if (myStor.commandLine[i] != '.') {
                                char *punctu = &myStor.commandLine[i];
                                deletePunctuationMark(1, punctu, 1);
                                i += 2;
                            } else {
                                char *punctu = new char[3]{'.','.','.'};
                                deletePunctuationMark(1, punctu, 3);
                                i += 2;
                            }
                        }
                    } else if (myStor.blockerP2 == 0){//&& blockerP2 == 0
                        i += 4;
                        myStor.pos = myStor.commandLine[i + 2] - 48;// позиция
                        i += 2;
                        length = 0;
                        while (myStor.commandLine[i + 2] != ',') {
                            myStor.newWord[length] = myStor.commandLine[i + 2];
                            length++;
                            i++;
                        }
                        nwLength = length;
                    }
                }
            }
        }
    }


    // / isDeleteAll - если не 0 то удалить только punctMarc,
    // иначе удалить все, модифицирует punctuation массив, заменяя выбранные знаки пробелом
    void deletePunctuationMark(int isDeleteAll, char *punctMarc, int length) {
        if (isDeleteAll == 0)
            for (int i = 0; i < pCounter; ++i) {
                *punctuation[i] = ' ';
            }
        else if (length == 1) {
            for (int i = 0; i < pCounter; ++i) {
                if (*punctuation[i] == punctMarc[0]) *punctuation[i] = ' ';
            }
        } else
            for (int i = 0; i < pCounter; ++i) {
                if (*punctuation[i] == punctMarc[0])
                    *punctuation[i] = ' ';//затираем многоточие
            }
            myStor.blockerP6 = 1;
    }

    void startOnThisWord(char *word, int length) {
        if (wordLength[0] == length) {
            int controller = 0;
            char *nWord = sentenceWith[0];
            for (int i = 0; i < wordLength[0]; ++i) {
                if (nWord[i] != word[i]) break;
                else controller++;
            }
            if (controller == wordLength[0]) {
                startOnThisWordNumber = myStor.sentenceCounter;
            }
        }
    }//H2

    void usingSumbolSequence(char *wordUse, int length) {
        int counter = 0;
        for (int i = 0; i < sWCounter; ++i)//перебор слов
        {
            if (wordLength[i] > length) {
                char *nWord =  sentenceWith[i];
                for (int j = 0; j < wordLength[i]; ++j) {
                    if (nWord[j] == wordUse[counter]) counter++;
                    else if(length == counter) break;
                    else counter = 0;
                }
                if (counter == length) {
                    myStor.word = nWord;
                    myStor.blockerC7 = -1;
                    myStor.wLength = wordLength[i];
                    return;
                } else counter = 0;
            }
        }
    }//c7

    // здесь я возвращаю адрес первого вхождения указанного слова

    // этот метод записывает результат в файл
    // навесить блокеры, если слово есть, а блокер 0
    void printer() {
        int positionCorr = 0;// завожу корректировки по знакам пунктуации
        ofstream out;               // поток для записи
        out.open(myStor.nameOfFile, ios::app);// окрываем файл для записи
        if (out.is_open()) {
            for (int i = 0; i < sWCounter; ++i) {
                if (wordLength[i] == 1) {
                    char *wordOne = sentenceWith[i];
                    if (wordOne[0] != ' ') {
                        if (punctuationChecker(wordOne[0]))positionCorr++;
                        else if (wordOne[0] == '.')positionCorr++;
                        else out << ' ';
                        out << wordOne[0];
                    }
                    delete[] wordOne;
                }
                int tmp = i - positionCorr + 1;
                if (myStor.pos == tmp && myStor.blockerP2 == 0)//проверка для пункта р2
                {
                    if (myStor.pos - 1 != 0) out << " ";
                    for (int j = 0; j < nwLength; ++j) {
                        out << myStor.newWord[j];
                    }
                    if (myStor.pos - 1 == 0) out << " ";
                    myStor.blockerP2 = 1;//блокирую метод p2
                }

                if (myStor.blockerH2 == 0 && startOnThisWordNumber != 0) {
                    out << "->" << " ";
                    myStor.blockerH2 = 1;
                }

                if ((wordLength[i]) != 3 && wordLength[i] != 1) {
                    if (i != 0) out << " ";
                    char *insWord = sentenceWith[i];
                    for (int k = 0; k < wordLength[i]; ++k) {
                        out << insWord[k];
                    }
                }// если слово не походит на знак пунктуации, то вывожу

                if ((*sentenceWith[i] - 48) == ' ') positionCorr++;
                if (wordLength[i] == 3) {
                    char *check = sentenceWith[i];
                    if (check[0] != ' ' && check[0] != '.') {
                        if (i != 0) out << ' ';
                    }
                    for (int j = 0; j < 3; ++j) {
                        out << check[j];
                    }
                }
            }
            out << '\n';
        }
        out.close();
        delete [] sentenceWith;
        delete [] punctuation;
        delete [] wordLength;
    }
};


int main() {
    string nameOfFile;
    cout << "Hello!\n"
            "This program can:\n"
            "R.2) insert in the sentence a new word before the given word,\n"
            "R.6) remove the punctuation mark (indicated and / or all) in the sentence.\n"
            "Indication of a specific sentence:\n"
            "H.2) a sentence starting with the specified word,\n"
            "Indication of a given word:\n"
            "C.7) containing a given sequence of characters\n"
            "to run programs \";\n"
            "To start it is necessary:\n"
            "1) enter the file from where you will take the sentences (each sentence from a new line),\n"
            " and on the next line, the file where you will display the sentences (these files must be different!)\n"
            "2) enter a sequence of commands separated by commas\n"
            "For C7 - C.7.ing, where C.7 is the command number, and ing is what we will look for\n"
            "For H.2 - H.2.hello, - where H.2 is the command number and hello is what we are looking for\n"
            "For P.6 - P.6.2.;, - where P.6 is the number of the command,\n"
            " 2 is the number of the sentence,; is the sign we are about to remove.\n"
            " Instead, you can put 0, then the program will delete all characters\n"
            "For P.2 - P.2.2.1.trtrtr, - where P.2 is the command number, 2 is the sentence number,\n"
            "1 is the word before which a new word must be inserted.\n"
            "The most important:\n"
            "1) each sentence from a new line\n"
            "2) each line of commands should end with a comma\n"
            "3) input and output files must be different\n";
    cin >> nameOfFile; // where i take text
    cin >> myStor.nameOfFile; // where i write answer
    cin >> myStor.commandLine;
    ifstream in;
    in.open(nameOfFile);
    string line;
    if (in.is_open()) {
        while (getline(in, line)) {
            myStor.sentenceCounter++;
            new Sentence(line);
            if (myStor.blockerC7 == -1) {
                ofstream out;               // поток для записи
                out.open(myStor.nameOfFile, ios::app);// окрываем файл для записи
                if (out.is_open()) {
                    out << "c7 result is: ";
                    for (int j = 0; j < myStor.wLength; ++j) {
                        out << myStor.word[j];
                    }
                    out << '\n';
                    myStor.blockerC7 = 1;
                    delete [] myStor.word;
                }
                out.close();
            }
        }
    }
    in.close();
}
