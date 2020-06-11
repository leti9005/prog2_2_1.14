#include <iostream>
#include <fstream>

using namespace std;
/** Текст представляет собой последовательность отдельных предложений, содержащих слова и знаки пунктуации.
 * udp пока работает так, что каждое предложение с новой строки
 * Преобразовать текст в соответствии с последовательностью команд редактирования,
 * которые должны позволять вставлять, удалять и заменять заданные слова в определенных предложениях.
 * Команды редактирования:
 * Р.2) вставить в предложении новое слово перед заданным словом,
 * Р.6) удалить в предложении знак пунктуации (указанный и/или все).
 * Указание определенного предложения:
 * П.2) предложение, начинающееся с указанного слова,
 * Указание заданного слова:
 * С.7) содержащее заданную последовательность символов
 */

struct MyStor {//for save all inform
    string nameOfFile;
    string commandLine;//line with command   6
//char *sentenceFirst;//first elem on all sentence
    int sFCounter = 0;
    char *sentenseLast;//указатель на последний элемент в предложении
// (можно реализвать при необходимости в методе split)
};

MyStor myStor;


class Sentence//новое предложение
{
public:
    int blocker = 0;//если 0 - просто работаем, метода П 2 небыло
    int *sentenceWith;// указатель на начало слов и знаков пунктуации
    int sWCounter = 0;// подсчет количества элементов в sentenceWith
    int *punctuation;// указатель на знаки пунктуации
    int *wordLength;// для sentenceWith
    int pCounter = 0;// подсчет количества элементов в punctuation
    //int *clearSentence;// слова без знаков пунктуации
    //int cSCounter = 0;// подсчет количества элементов в clearSentence
    //int *lastPunctuation; // храню знаки препинания конца
    //int lPCounter = 0;//подсчет количества элементов в lastPunctuation

    //для создания нового предложения просто ввожу строку
    Sentence(string line) { splitter(line); }

    //метод парсит предложение на слова и вызывает метод добавления
    void splitter(string line) {
        int totalWordCount = 0;// количество букв в слове, на один меньше для записи в массив
        char *word = new char[5];
        //int flag = 0; //первое слово
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == ' ') {
                char newWord[totalWordCount];
                for (int j = 0; j < totalWordCount; ++j) {
                    newWord[j] = word[j];
                }
                add(newWord);
                //  flag = 1;
                totalWordCount = 0;
            } else {
                word = new char[totalWordCount];
                word[totalWordCount] = line[i];
                totalWordCount++;
            }
        }
        sWCounter++; //увеличиваю счетчик элементов предложения
        myStor.sFCounter++;
        printer("wwor",4);
        commandParser();
    }

    // метод добавляет новое слово и заводит указатель на его начало в sentenceWith, punctuation, clearSentence
    void add(char word[]) {
        int length = 0;
        int testLength = 1;
        //если последняя буква в слове - знак препинания, делаю длину слова меньше на 1
        if (!punctuationChecker(word[sizeof(word) - 1]))
            length = sizeof(word);
        else if (word[sizeof(word) - 1] == '.' && word[sizeof(word) - 2] == '.')// проверяю на многоточие
            length = sizeof(word) - 3;
        else length = sizeof(word) - 1;
        char newWord[length] ;
        for (int j = 0; j < length; ++j) {
            newWord[j] = word[j];
        }
        // указатель на начало слова завожу в необходимые массивы
        sentenceWith[sWCounter] = *newWord;// в массив указателей вводим новый указатель на слово
        wordLength[sWCounter] = length;
        sWCounter++;
        // clearSentence[cSCounter] = *newWord;
        // countWord++;
        /**
        if (flag == 0) //если это первое слово - завожу указатель на него в соответствующий массив указателей
        {
            myStor.sentenceFirst[myStor.sFCounter] = *newWord;
            myStor.sFCounter++;
        }
        **/
        if (length++ == sizeof(word)) //проверяю, был ли в конце знак препинания не равный многоточию
        {
            char punct[1];
            punct[0] = word[length];
            sentenceWith[sWCounter] = *punct;

            wordLength[sWCounter] = 1;
            sWCounter++;
            punctuation[pCounter] = *punct;
            pCounter++;
        } else if ((length += 3) == sizeof(word)) {
            char punct[3];
            punct[0], punct[1], punct[2] = '.';
            sentenceWith[sWCounter] = *punct;
            wordLength[sWCounter] = 3;
            sWCounter++;
            punctuation[pCounter] = *punct;
            pCounter++;
        }
    }

    // check punctuation mark
    // if the char is a punctuation mark function return true, and return false if is not
    bool punctuationChecker(char mark) {
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
        cout << "hello";
    }
    //P6

    // / isDeleteAll - если 0 то удалить только punctMarc,
    // иначе удалить все, модифицирует punctuation массив, заменяя выбранные знаки пробелом
    void deletePunctuationMark(int isDeleteAll, char punctMarc) {
        if (isDeleteAll != 0)
            for (int i = 0; i < pCounter; ++i) {
                punctuation[i] = ' ';
            }
        else
            for (int i = 0; i < pCounter; ++i) {
                if (punctuation[i] == punctMarc) punctuation[i] = ' ';
            }
    }

    void startOnThisWord(char* word)
    {
        for (int i = 0; i < wordLength[0]; ++i) {
            if (word[i] == &(sentenceWith[0])[i])
        }

    }//П2

    void usingSumbolSequence() { }//c7

    // здесь я возвращаю адрес первого вхождения указанного слова
    char *scanner(char *word) { return 0; }

    // этот метод записывает результат в файл
    void printer(string word, int position) {
        int positionCorr = position;// завожу корректировки по знакам пунктуации
        ofstream out;               // поток для записи
        out.open(myStor.nameOfFile);// окрываем файл для записи
        if (out.is_open()) {
            int checker = wordLength[0];

            for (int i = 0; i < sWCounter; ++i) {
                if (position != 0 && positionCorr == i) out << " " << word;//проверка для пункта
                else if (reinterpret_cast<int>(&wordLength[i]) != 3 && wordLength[i] != 1)
                    for (int k = 0; k < wordLength[i]; ++k) {
                        out << " " << &sentenceWith[i];
                    }
                    // если слово не походит на знак пунктуации, то вывожу
                else if (wordLength[i] == 3) {
                    for (int j = 0; j < 3; ++j) {

                    }
                }
            }

        }
        out.close();

    }


};


int main() {
    string nameOfFile;
    cout << "sompth hello";
    cin >> nameOfFile; // where i take text
    cin >> myStor.nameOfFile; // where i write answer
    cin >> myStor.commandLine;
    ifstream in;
    in.open(nameOfFile);
    string line;
    if (in.is_open()) {
        while (getline(in, line)) {
            std::cout << line << std::endl;
            new Sentence(line);
        }
    }
    in.close();
    //in this plase user write all command/ command logic : command,command. example: c.7,p.2,
    //подразумевается, что в многоточии 3 точки

}
