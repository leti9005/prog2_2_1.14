#include <iostream>
#include <fstream>
#include <string>

using namespace std;
/**
Р.2) вставить в предложении новое слово перед заданным словом,
Р.6) удалить в предложении знак пунктуации (указанный и/или все).
Указание определенного предложения:
П.2) предложение, начинающееся с указанного слова,
Указание заданного слова:
С.7) содержащее заданную последовательность символов
**/
struct MyStor {//for save all inform
    string nameOfFile;
    int sentenceCounter = 0;//порядковый номер предложения
    string commandLine;//line with command   6
    int pos = -1;// P2 position
    int blockerP2 = 0;
    int blockerH2 = 0;
    int blockerC7 = 0;
    int blockerP6 = 0;
    char *newWord = new char[CHAR_MAX];// P2 word
    int nwLength = 0;// р2 newWord length
    char *word = new char[CHAR_MAX];//C7
    int wLength = 0;//c7 word length
};

struct nexSumb//структура хранения всех слов и символов
{
    nexSumb* nWord;// указатель на следующее слово
    int length = 0;// длина слова
    char sent[20];// само слово
    int flag = 0;//punctuation checker
    //0 - не пунктуация, 1 - пунктуаця, 2 - многоточие
    int deleted = 0; // 0 - не удалено, 1 - удалено
};

MyStor myStor;


class Sentence//новое предложение
{
public:

   // char **sentenceWith = new char *[CHAR_MAX];// указатель на начало слов и знаков пунктуации
    int sWCounter = 0;// подсчет количества элементов в sentenceWith
    //char **punctuation = new char *[CHAR_MAX];// указатель на знаки пунктуации
    //int *wordLength = new int[INT16_MAX];// для sentenceWith

    //int pCounter = 0;// подсчет количества элементов в punctuation
    nexSumb *firstWord; // указатель на первое слово


    //для создания нового предложения просто ввожу строку
    Sentence(string line) { splitter(line); }

    //метод парсит предложение на слова и вызывает метод добавления
    void splitter(string line) {
        int totalWordCount = 0;// количество букв в слове, на один меньше для записи в массив
        int i = 0;
        char *wordFast = new char [line.length()];
        while (line.length() > i && line[i] != ' ')
        {
            wordFast[i] = line[i];
            i++;
            totalWordCount++;
        }
        nexSumb tmp = add(wordFast,totalWordCount);
        int j = totalWordCount + 1;
        totalWordCount = 0;
        int flag = 0;

        nexSumb *prevWord = new nexSumb;
        for (int i = j; i < line.length(); ++i) {
            if(flag == 0)
            {
                prevWord = &tmp;
                flag = 1;
            }
            if (line[i] == ' ' || j++ == line.length())
            {
                nexSumb temp;
                temp = add(wordFast, totalWordCount);// связываю его с предыдущим элементом
                prevWord->nWord = &temp;
                prevWord = &temp; // элемент становится предыдущим
                totalWordCount = 0;

            }
            else {
                wordFast[totalWordCount] = line[i];
                totalWordCount++;
            }
        }
        commandParser();
        strPrinter();
        delete[] wordFast;
    }

    // метод добавляет новое слово в структуру, если была пунктуация, то добавится и она отдельным словом. Возвращает
    // указатель на это слово
    nexSumb add(char *wordAdd, int length) {
        int fastLength = length;// хранение первоначального размера
        nexSumb *sumb = NULL;// знак препинания если будет
        //если последняя буква в слове - знак препинания, делаю длину слова меньше на 1

        if (punctuationChecker(wordAdd[length - 1]) || wordAdd[length - 1] == '.') {
            if (wordAdd[length - 2] == '.')// проверяю на многоточие
            {
                nexSumb nexSumb;
                nexSumb.flag = 2;// многоточие
                nexSumb.length = 3;
                nexSumb.sent[0] = '.';
                nexSumb.sent[1] = '.';
                nexSumb.sent[2] = '.';
                sumb = &nexSumb;
                length -= 3;
                sWCounter++;
            } else { // если не многоточие, добавляю
                nexSumb nexSumb;
                nexSumb.flag = 1;
                nexSumb.length = 1;
                nexSumb.sent[0] = wordAdd[length - 1];
                sumb = &nexSumb;
                length -= 1;
                sWCounter++;
            }
        }//если была пунктуация я создал новую структуру
        nexSumb word;
        for (int j = 0; j < length; ++j) {
            word.sent[j] = wordAdd[j];
        }// основное добавление слова
        sWCounter++;
        word.length = length;
        if (length < fastLength) //проверяю, был ли в конце знак препинания и добавляю в слово как следующий элемент
        {
            word.nWord = sumb;
            firstWord = &word;
            return *sumb;
        }
        if (sWCounter == 0) firstWord = &word;
        return word;
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
                char *nxWord = new char[length];
                length = 0;
                while (myStor.commandLine[i] != ',') {
                    nxWord[length] = myStor.commandLine[i];
                    length++;
                    i++;
                }
                startOnThisWord(nxWord, length);
                delete[] nxWord;
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
                char *nxWord = new char[length];
                length = 0;
                while (myStor.commandLine[i] != ',') {
                    nxWord[length] = myStor.commandLine[i];
                    length++;
                    i++;
                }
                usingSumbolSequence(nxWord, length);
                myStor.nwLength = length;
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
                        myStor.nwLength = length;
                    }
                }
            }
        }
    }


    // / isDeleteAll - если не 0 то удалить только punctMarc,
    // иначе удалить все, модифицирует punctuation массив, заменяя выбранные знаки пробелом
    void deletePunctuationMark(int isDeleteAll, char *punctMarc, int length) {
        nexSumb *nexSumb = firstWord->nWord;// ссылаю сразу на 2 элемент, если что удаляю его
        if(firstWord->flag != 0 && length == firstWord->length)
            if(isDeleteAll == 0 || firstWord->sent[0] == '.' || firstWord->sent[0] == *punctMarc)
            {
                firstWord->deleted = 1;// удалил знак
            }
        for (int i = 1; i < sWCounter; ++i)
        {// прохожу по всем элементам в
            if(nexSumb->flag != 0 && length == nexSumb->length)
                if(isDeleteAll == 0 || nexSumb->sent[0] == '.' || nexSumb->sent[0] == *punctMarc)
                {
                    firstWord->deleted = 1;// удалил знак
                }
        }
            myStor.blockerP6 = 1;
    }

    void startOnThisWord(char *word, int length) {
        if (firstWord->length == length) {
            int controller = 0;
            for (int i = 0; i < firstWord->length; ++i) {
                if (firstWord->sent[i] != word[i]) break;
                else controller++;
            }
            if (controller == length) {
                myStor.blockerH2 = 1;
            }
        }
    }//H2

    void usingSumbolSequence(char *wordUse, int length) {
        int counter = 0;
        nexSumb *nexSumb = firstWord->nWord;// ссылаю сразу на 2 элемент, если что удаляю его
        if(firstWord->flag != 0 && length <= firstWord->length)
            {
                for (int i = 0; i < firstWord->length; ++i) {
                    if (firstWord->sent[i] == wordUse[counter]) counter++;
                    else if(length == counter) break;
                    else counter = 0;
                }
                if (counter == length) {
                    myStor.word = firstWord->sent;
                    myStor.blockerC7 = -1;
                    myStor.wLength = firstWord->length;
                    return;
                }
            }

        for (int i = 1; i < sWCounter; ++i)
        {// прохожу по всем элементам
            if(nexSumb->flag != 0 && length <= nexSumb->length)
                {
                    for (int j = 0; j < nexSumb->length; ++j) {
                        if (nexSumb->sent[j] == wordUse[counter]) counter++;
                        else if(length == counter) break;
                        else if (length-=i < length) break;
                        else counter = 0;
                    }
                }
        }

        if (counter == length) {
            myStor.word = firstWord->sent;
            myStor.blockerC7 = -1;
            myStor.wLength = firstWord->length;
            return;
        }

    }



    // здесь я возвращаю адрес первого вхождения указанного слова
    void strPrinter() {
        int counter = 0; // подсчет вывода слов для пункта P2
        ofstream out;// поток для записи
        nexSumb *nexSumb = firstWord;// cледующий элемент
        out.open(myStor.nameOfFile, ios::app);// окрываем файл для записи
        if (out.is_open()) {
            for (int i = 0; i < sWCounter; ++i) {
                if (myStor.blockerH2 == 1) {
                    out << "->" << " ";
                    myStor.blockerH2 = 0;
                }

                // вывод для метода Н2
                if (myStor.pos == counter && myStor.blockerP2 == 0)//проверка для пункта р2
                {
                    if (myStor.pos - 1 != 0) out << " ";
                    for (int j = 0; j < myStor.nwLength; ++j) {
                        out << myStor.newWord[j];
                    }
                    if (myStor.pos - 1 == 0) out << " ";
                    myStor.blockerP2 = 1;//блокирую метод p2
                }
                    // вывод для метода Р2

                else // вывод для всего остального
                {
                    if (nexSumb->deleted == 0) // слово не удалено
                    {
                        if (nexSumb->flag == 0) {
                            counter++;
                            if (i != 0)cout << " ";// если не первое слово или не пунктуация выведу пробел
                        }

                        for (int j = 0; j < nexSumb->length; ++j) {
                            out << nexSumb->sent[j];
                        }


                    } else // если удалено
                    {
                        nexSumb = nexSumb->nWord; // передаю указатель на следующее слово
                    }
                }
            }
        }
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
// в отчете только перемахнуть код на новый

// глянуть флаги пунктуации в ADD