// Задание: программа, определеяющая корректность применения одномерных массивов

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ReadingFile {
public:
    static vector<string> readingFile() { // статический метод, который возвращает вектор строк
        string fileName; //для чтения содержимого файла построчно
        vector<string> linesOfCode; // пустой вектор строк, в который будут добавляться строки кода из файла
        ifstream file("code.txt"); // открытие файла 
        if (file.is_open()) {
            while (getline(file, fileName)) { // из файла считываются строки по очереди
                linesOfCode.push_back(fileName); // каждая строка добавляется в вектор linesOfCode.
            }
            file.close();
        } else {
            cout << "Ошибка: Не удалось открыть файл" << endl;
        }
        return linesOfCode; //возвращает вектор, содержащий строки кода из файла
    }
};

class CheckArray {
private:
    static bool findArray(const string& s) {
        size_t openBracketPosition = s.find('['); // ищет позицию открывающей скобки [
        return (openBracketPosition != string::npos);
    }

    static void printError(int lineNumber, const string& lineOfCode, const string& errorMessage) {
        cerr << "Ошибка в строке " << lineNumber << ": " << lineOfCode << " --- " << errorMessage << endl;
    }

    static bool checkTypeOfArray(const string& s) {
        if (s == "int" || s == "float" || s == "double" || s == "char" || s == "bool") { //проверяет, является ли строка s одним из допустимых типов данных для массива
            return true;
        }
        return false;
    }

    static bool checkNameOfArray(const string& s) {
        if (s.empty() || !isalpha(s[0]) && s[0] != '_') { // проверка на пустосту, первый символ не цифра и не равен подчеркиванию
            return false;
        }
        for (int i = 0; i < s.length(); ++i) { // проходим по каждой строке 
            char c = s[i];
            if (!isalnum(c) && c != '_') { //проверяем, что символ является буквой, цифрой или символом _
                return false;
            }
        }
        return true;
    }

    static bool isKeyword(const string& s) { // ищем совпадения с ключевым словом
        if (s == "auto" || s == "bool" || s == "break" || s == "case" || s == "catch"
        || s == "char" || s == "class" || s == "const" || s == "continue" || s == "do"
        || s == "double" || s == "else" || s == "enum" || s == "extern" || s == "false"
        || s == "float" || s == "for" || s == "goto" || s == "if" || s == "int"
        || s == "long" || s == "namespace" || s == "new" || s == "nullptr" || s == "operator"
        || s == "private" || s == "protected" || s == "public" || s == "return" || s == "short"
        || s == "signed" || s == "sizeof" || s == "static" || s == "struct" || s == "switch"
        || s == "template" || s == "this" || s == "throw" || s == "true" || s == "try"
        || s == "typedef" || s == "typename" || s == "union" || s == "unsigned" || s == "void"
        || s == "volatile" || s == "while") {
        return true;
    }
        return false;
    }

public:
    static void checkArrays(vector<string>& arr) {
        bool hasErrors = false; // означает, что пока ошибки не обнаружены

        for (int i = 0; i < arr.size(); ++i) {
            if (!findArray(arr[i])) { // если не найден массив, то пропускается
                continue; 
            }

            string line = arr[i]; 

            size_t commentPosition = line.find("//"); // находим позицию //
            size_t blockCommentPosition = line.find("/*");
            if (commentPosition != string::npos || blockCommentPosition != string::npos) { // string::npos - значит не найдено // проверяет, есть ли хотя бы один из типов комментария в строке line
                if (commentPosition < line.find('[') || (blockCommentPosition != string::npos && blockCommentPosition < line.find('['))) { // и находится ли коммент после открывающей скобки
                    continue;
                }
            }
            size_t spacePosition = line.find(' ');
            size_t openingBracketPosition = line.find('[', spacePosition); // возвращает позицию первого вхождения символа [ в строке line, начиная с позиции spacePosition
            
            if (spacePosition == string::npos || openingBracketPosition == string::npos) {
                printError(i + 1, arr[i], "не обнаружен массив");
                hasErrors = true;
                continue;
            }

            size_t closingBracketPosition = line.find(']', openingBracketPosition + 1);

            if (closingBracketPosition == string::npos) {
                printError(i + 1, arr[i], "не обнаружено закрывающей скобки для массива");
                hasErrors = true;
                continue;
            }
            size_t semicolonPosition = line.find(';', closingBracketPosition + 1);
            if (semicolonPosition == string::npos) {
                printError(i + 1, arr[i], "не обнаружена точка с запятой после закрывающей скобки для массива");
                hasErrors = true;
                continue;
            }

            string arrayContent = line.substr(openingBracketPosition + 1); // возвращает подстроку arrayContent, которая является частью строки line начиная с позиции openingBracketPosition + 1 и до конца строки
            int openBracketCount = 1;
            int closeBracketCount = 0;
            for (int j = 0; j < arrayContent.length(); j++) {
                char c = arrayContent[j];
                if (c == '[') {
                    openBracketCount++;
                } else if (c == ']') {
                    closeBracketCount++;
                }

                if (closeBracketCount > openBracketCount) {
                    printError(i + 1, arr[i], "лишняя закрывающая скобка для массива");
                    hasErrors = true;
                    break;
                }
            }

            if (closeBracketCount < openBracketCount) {
                printError(i + 1, arr[i], "лишняя открывающая скобка для массива");
                hasErrors = true;
            } 

            string arrayBetweenBrackets = line.substr(openingBracketPosition + 1, closingBracketPosition - openingBracketPosition - 1); // создание подстроки arrayBetweenBrackets из строки line, начиная с позиции openingBracketPosition + 1 и до позиции closingBracketPosition - 1
            if (arrayBetweenBrackets.empty()) {
                printError(i + 1, arr[i], "не задан размер массива");
                hasErrors = true;
                continue;
            }
            try {
                int size = stoi(arrayBetweenBrackets);
            } catch (const invalid_argument&) {
                printError(i + 1, arr[i], "размер массива указан не в числах");
                hasErrors = true;
                continue;
            }

            string dataType = line.substr(0, spacePosition);
            string arrayName = line.substr(spacePosition + 1, openingBracketPosition - spacePosition - 1);

            if (!checkTypeOfArray(dataType)) {
                printError(i + 1, arr[i], "некорректный тип данных");
                hasErrors = true;
            }

            if (!checkNameOfArray(arrayName)) {
                printError(i + 1, arr[i], "некорректное имя массива");
                hasErrors = true;
            }

            if (isKeyword(arrayName)) {
                printError(i + 1, arr[i], "использование ключевого слова в имени массива");
                hasErrors = true;
            }

            size_t openingCurlyBracketPosition = line.find('{', closingBracketPosition + 1);
            size_t closingCurlyBracketPosition = line.find('}', openingCurlyBracketPosition + 1);

            if (openingCurlyBracketPosition == string::npos || closingCurlyBracketPosition == string::npos || openingCurlyBracketPosition > closingCurlyBracketPosition) {
                printError(i + 1, arr[i], "не обнаружена допустимая инициализация массива");
                hasErrors = true;
                continue;
            }
        }

        if (!hasErrors) {
            cout << "Проверка завершена. Ошибок не обнаружено." << endl;
        }
    }


};

int main() {
    vector<string> linesOfCode = ReadingFile::readingFile();
    for (int i = 0; i < linesOfCode.size(); i++) {
        cout << linesOfCode.at(i) << '\n';
    }
    CheckArray::checkArrays(linesOfCode);

    return 0;
}
