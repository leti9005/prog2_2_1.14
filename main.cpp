#include <iostream>
#include <fstream>

using namespace std;
/** Текст представляет собой последовательность отдельных предложений, содержащих слова и знаки пунктуации.
 * Преобразовать текст в соответствии с последовательностью команд редактирования,
 * которые должны позволять вставлять, удалять и заменять заданные слова в определенных предложениях.
 * Команды редактирования:
 * Р.2) вставить в предложении новое слово перед заданным словом,
 * Р.6) удалить в предложении знак пунктуации (указанный и/или все).
 * Указание определенного предложения:
 * H.2) предложение, начинающееся с указанного слова,
 * Указание заданного слова:
 * С.7) содержащее заданную последовательность символов
 * udp пока работает так, что каждое предложение с новой строки
 * udp 2 пример команда.номер.(особый указатель)
 * udp 3 П2 заменил на H2
 */

struct MyStor {//for save all inform
    string nameOfFile;
    int sentenceCounter = 0;//порядковый номер предложения
    string commandLine;//line with command   6
//char *sentenceFirst;//first elem on all sentence
    //int sFCounter = 0;
    char *sentenseLast;//указатель на последний элемент в предложении
// (можно реализвать при необходимости в методе split)
};

MyStor myStor;


class Sentence//новое предложение
{
public:
    int blockerP2 = 0;
    int blockerH2 = 0;
    int blockerC7 = 0;
    char *sentenceWith = new char[CHAR_MAX];// указатель на начало слов и знаков пунктуации
    int sWCounter = 0;// подсчет количества элементов в sentenceWith
    int *punctuation = new int[INT16_MAX];// указатель на знаки пунктуации
    int *wordLength = new int[INT16_MAX];// для sentenceWith
    char *newWord = new char[CHAR_MAX];// P2 word
    int pos = 0;// P2 position
    int pCounter = 0;// подсчет количества элементов в punctuation
    int startOnThisWordNumber = 0; //h2 result
    char *word = new char[CHAR_MAX];//C7

    //int *clearSentence;// слова без знаков пунктуации
    //int cSCounter = 0;// подсчет количества элементов в clearSentence
    //int *lastPunctuation; // храню знаки препинания конца
    //int lPCounter = 0;//подсчет количества элементов в lastPunctuation

    //для создания нового предложения просто ввожу строку
    Sentence(string line) { splitter(line); }

    //метод парсит предложение на слова и вызывает метод добавления
    void splitter(string line) {
        int totalWordCount = 0;// количество букв в слове, на один меньше для записи в массив
        char *wordFast = new char[totalWordCount];
        //int flag = 0; //первое слово
        for (int i = 0; i < line.length(); ++i) {
            if (line[i] == ' ') {
                add(wordFast,totalWordCount);
                //  flag = 1;
                totalWordCount = 0;

            } else {
                wordFast[totalWordCount] = line[i];
                totalWordCount++;
            }
        }
        add(wordFast,totalWordCount);
        //  flag = 1;
       // totalWordCount = 0;
        //sWCounter++; //увеличиваю счетчик элементов предложения ????????????????????????????????????????????????????
        commandParser();
        printer();
        delete[] wordFast;
    }

    // метод добавляет новое слово и заводит указатель на его начало в sentenceWith, punctuation, clearSentence
    void add(char* wordAdd, int length) {
         int fastLength = length;
        //если последняя буква в слове - знак препинания, делаю длину слова меньше на 1
        if (punctuationChecker(wordAdd[length - 1]))
            length -= 1;
        else if (wordAdd[length - 1] == '.')
            {
             if(wordAdd[length - 2] == '.')// проверяю на многоточие
            length -= 3;
            else length -= 1;
            }
        char *nWord = new char[length];
        for (int j = 0; j < length; ++j) {
            nWord[j] = wordAdd[j];
        }
        // указатель на начало слова завожу в необходимые массивы
        sentenceWith[sWCounter] = *nWord;// в массив указателей вводим новый указатель на слово
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
        length++;
        if (length == fastLength) //проверяю, был ли в конце знак препинания не равный многоточию
        {
            char punct[1];
            punct[0] = wordAdd[length];
            sentenceWith[sWCounter] = *punct;
            wordLength[sWCounter] = 1;
            sWCounter++;
            punctuation[pCounter] = *punct;
            pCounter++;
        }
        else {
            length += 2;
            if (length == fastLength)
            {
                char punct[3];
                punct[0], punct[1], punct[2] = '.';
                sentenceWith[sWCounter] = *punct;
                wordLength[sWCounter] = 3;
                sWCounter++;
                punctuation[pCounter] = *punct;
                pCounter++;
            }
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
    void commandParser()
    {
        for (int i = 0; i < myStor.commandLine.length(); ++i)
        {
           int length = 0;
           int tempI;
           if(myStor.commandLine[i] == 'H' && blockerH2 == 0)
           {
               tempI = i;
               i += 2;//+ 2 для пропуска номера и точки
               while (myStor.commandLine[i] != ',')
               {
                   length++;
                   i++;
               }
               i = tempI;
               char *nWord = new char[length];
               length = 0;
               while (myStor.commandLine[i] != ',')
               {
                   nWord[length] = myStor.commandLine[i];
                   length++;
                   i++;
               }
               startOnThisWord(nWord, length);

               delete [] nWord;
           }
           if(myStor.commandLine[i] == 'C' && blockerC7 == 0) {
               tempI = i;
               i += 2;//+ 2 для пропуска номера и точки
               while (myStor.commandLine[i] != ',')
               {
                   length++;
                   i++;
               }
               i = tempI;
               char *nWord = new char[length];
               length = 0;
               while (myStor.commandLine[i] != ',')
               {
                   nWord[length] = myStor.commandLine[i];
                   length++;
               }
               usingSumbolSequence(nWord, length);
               return;
           }
           if(myStor.commandLine[i] == 'P') {
               if(myStor.commandLine[i + 2] == '6'&& myStor.commandLine[i+3] == myStor.sentenceCounter)
               {
                   i+=3;
                   if(myStor.commandLine[i + 2] == 0) {
                       i+=2;
                       deletePunctuationMark(0, reinterpret_cast<char *>('r'), 0);
                   }
                    else {
                        deletePunctuationMark(1, reinterpret_cast<char *>(myStor.commandLine[i + 2]), 1);
                        i+=2;
                    }
               }
               else if(myStor.commandLine[i+2] == myStor.sentenceCounter && blockerP2 == 0) {
                   i+=2;
                   pos = myStor.commandLine[i + 2];// позиция
                   i+2;
                   length = 0;
                   while (myStor.commandLine[i + 1] != ',')
                   {
                       newWord[length] = myStor.commandLine[i + 1];
                       length++;
                       i++;
                   }
               }
           }
        }
    }


    // / isDeleteAll - если не 0 то удалить только punctMarc,
    // иначе удалить все, модифицирует punctuation массив, заменяя выбранные знаки пробелом
    void deletePunctuationMark(int isDeleteAll, char* punctMarc, int length) {
        if (isDeleteAll == 0)
            for (int i = 0; i < pCounter; ++i) {
                punctuation[i] = ' ';
            }
        else if(length == 1)
        {
            for (int i = 0; i < pCounter; ++i) {
                if (punctuation[i] == punctMarc[0]) punctuation[i] = ' ';
            }
        }
        else
            for (int i = 0; i < pCounter; ++i)
            {
                if (punctuation[i] == punctMarc[0]) punctuation[i] = ' ';//затираем многоточие
            }
    }

    char* startOnThisWord(char* word, int length)
    {
        if (wordLength[0] == length) {
            int controller = 0;
            char *nWord = new char(wordLength[0]);
            nWord = &sentenceWith[0];
            for (int i = 0; i < wordLength[0]; ++i) {
                if (nWord[i] != word[i]) break;
                else i = controller;
            }
            if (controller == wordLength[0])
            {
              startOnThisWordNumber = myStor.sentenceCounter;
            }
        }
    }//H2

    void usingSumbolSequence(char *word, int length)
    {
        int counter = 0;
        for (int i = 0; i < sWCounter; ++i)//перебор слов
        {
          if(wordLength[i] > length)
          {
              char* nWord = new char [wordLength[i]];
              nWord = &sentenceWith[i];
              for (int j = 0; j < wordLength[i]; ++j)
              {
                  if (nWord[j] == word[counter]) counter++;
              }
              if(counter == length)
              {
                  word = nWord;
                  blockerC7 = -1;
              }
              else counter = 0;
          }
        }
    }//c7

    // здесь я возвращаю адрес первого вхождения указанного слова
    //char *scanner(char *word) { return 0; }

    // этот метод записывает результат в файл
    // навесить блокеры, если слово есть, а блокер 0
    void printer() {
        int positionCorr = pos;// завожу корректировки по знакам пунктуации
        ofstream out;               // поток для записи
        out.open(myStor.nameOfFile, ios::app);// окрываем файл для записи
        if (out.is_open()) {
            out << '\n';
            //int checker = wordLength[0];
            for (int i = 0; i < sWCounter; ++i) {
                if (pos != 0 && positionCorr == i)
                {
                    out << " " << &newWord;//проверка для пункта р2
                    blockerP2 = 1;//блокирую метод p2
                }
                if (blockerH2 == 0 && startOnThisWordNumber != 0)
                {
                    out << " " << startOnThisWordNumber;
                    blockerH2 = 1;
                }
                if (blockerC7 == -1)
                {
                    out << " " << &word;
                    blockerC7 = 1;
                }
                else if ((wordLength[i]) != 3 && wordLength[i] != 1)
                        out << " " << &sentenceWith[i];
                // если слово не походит на знак пунктуации, то вывожу
                else if (wordLength[i] == 1 && sentenceWith[i] != ' ') out << sentenceWith[i];
                else if (wordLength[i] == 3) {
                    char *check = new char[3];
                    check = &sentenceWith[i];
                    if(check[0] != ' ') out << sentenceWith[i];
                }
            }
        }
        out.close();
    }


};


int main() {
    string nameOfFile;
    cout << "sompth hello\n";
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
        }
    }
    in.close();
    //in this plase user write all command/ command logic : command.command. example: c.7,p.2,
    //подразумевается, что в многоточии 3 точки

}
//hello, this is a sentence 1.
//this is a sentence 2!
//Where are you, my darling?
//C:\tmp\test1.txt
//C:\tmp\test1.txt
//P.2.2.4.trtrtr,