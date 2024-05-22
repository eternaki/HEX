#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "String.h"



// Konstruktor
String::String() {
    data = new char[1];
    data[0] = '\0';
    size = 0;
}

// Konstruktor kopiujący
String::String(const String& other) {
    size = other.size;
    data = new char[size + 1];
    std::strcpy(data, other.data);
}

// Destruktor
String::~String() {
    delete[] data;
}


String& String::operator=(const char* str) {
    delete[] data;
    size = strlen(str);
    data = new char[size + 1];
    std::strcpy(data, str);
    return *this;
}

// Przypisanie
String& String::operator=(const String& other) {
    // Sprawdź, czy nie próbujemy przypisać obiekt do samego siebie
    if (this == &other) {
        return *this;
    }

    // Usuń poprzednie wartości pola str
    delete[] data;

    // Skopiuj wartości z innego obiektu
    size = other.size;
    data = new char[size + 1];
    std::strcpy(data, other.data);

    // Zwróć referencję do bieżącego obiektu
    return *this;
}

// Dodawanie
String String::operator+(const String& other) const {
    String result;
    result.size = size + other.size;
    result.data = new char[result.size + 1];
    std::strcpy(result.data, data);
    std::strcat(result.data, other.data);
    return result;
}

// Porównanie
bool String::operator==(const String& other) const {
    if (size != other.size) {
        return false;
    }
    return std::strcmp(data, other.data) == 0;
}

bool String::operator==(const char* s) const {
    return strcmp(data, s) == 0;
}


String& String::operator+=(const String& other) {
    char* new_data = new char[size + other.size + 1]; // +1 for the null terminator
    std::strcpy(new_data, data);
    std::strcat(new_data, other.data);

    delete[] data;
    data = new_data;
    size += other.size;

    return *this;
}

String& String::operator+=(const char other) {
    std::size_t other_size = std::strlen(&other);

    char* new_data = new char[size + other_size + 1]; // +1 for the null terminator
    std::strcpy(new_data, data);
    std::strcat(new_data, &other);

    delete[] data;
    data = new_data;
    size += other_size;

    return *this;
}


// Zwraca długość
int String::getLength() const {
    return size;
}

// Zwraca znak na danej pozycji
char String::operator[](int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// Dodaje znak na koniec
void String::pushBack(char c) {
    char* newData = new char[size + 2];
    std::strcpy(newData, data);
    newData[size] = c;
    newData[size + 1] = '\0';
    delete[] data;
    data = newData;
    size++;
}

// Czyści stringa
void String::clear() {
    delete[] data;
    data = new char[1];
    data[0] = '\0';
    size = 0;
}

// Zwraca wskaźnik na dane
const char* String::c_str() const {
    return data;
}

String String::convertCharArrayToString(char charArray[]) {
    String str;
    int i = 0;
    while (charArray[i] != '\0') {
        str.pushBack(charArray[i]);
        i++;
    }
    return str;
}

String String::substring(int start, int length) {
    String temp;
    // Sprawdź, czy argumenty są poprawne
    if (start < 0 || start > size || length < 0) {
        // Wypisz błąd i zwróć
        std::cout << "Niepoprawne argumenty metody substring()\n";
        //return 0;
    }

    // Oblicz indeks końca podciągu
    int end = start + length;

    // Sprawdź, czy indeks końca jest większy od rozmiaru napisu
    if (end > size) {
        end = size;
    }

    // Stwórz tymczasowy bufor na wynikowy podciąg
    char* buffer = new char[end - start + 1];

    // Skopiuj znaki z podciągu do bufora
    for (int i = start; i < end; i++) {
        buffer[i - start] = data[i];
    }

    // Dodaj znak końca napisu
    buffer[end - start] = '\0';

    // Zwolnij poprzednio przechowywany napis
    delete[] data;

    // Przypisz nowy napis
    data = buffer;

    // Zaktualizuj rozmiar napisu
    size = end - start;
    temp = temp.convertCharArrayToString(data);
    return temp;
}

int String::find(char c) const {
    for (int i = 0; i < size; i++) {
        if (data[i] == c) {
            return i;
        }
    }
    return -1;
}


int String::count(char c) const {
    int counter = 0;
    for (int i = 0; i < size; i++) {
        if (data[i] == c) {
            counter++;
        }
    }
    return counter;
}

// metoda zwracająca znak o podanym indeksie
char String::getChar(int index) const {
    if (index < 0 || index >= size) {
        return '\0';
    } else {
        return data[index];
    }
}

bool String::empty() const {
    return size == 0;
}

void String::erase(int position, int count)
{
    // Sprawdź, czy podana pozycja jest prawidłowa
    if (position >= size) {
        return;
    }

    // Sprawdź, czy liczba znaków do usunięcia nie wykracza poza długość łańcucha
    if (count > (size - position)) {
        count = size - position;
    }

    // Przesuń znaki o 'count' pozycji w lewo, aby zasłonić usuwane znaki
    for (int i = position; i < size - count; i++) {
        data[i] = data[i + count];
    }

    // Ustaw nową długość łańcucha
    size -= count;
    data[size] = '\0';
}

void String::trim(String& data) {
    int start = 0;
    while (data[start] == ' ' || data[start] == '\t' || data[start] == '\n') {
        start++;
    }
    int end = data.size - 1;
    while (end >= 0 && (data[end] == ' ' || data[end] == '\t' || data[end] == '\n')) {
        end--;
    }
    data.substring(start, end - start + 1);

}

int String::my_strlen(const char *str) {
    int len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

bool String::checkInt(char* data) {
    if (data == nullptr || data[0] == '\0') {
        return false; // nullptr lub pusty ciąg znaków nie jest liczbą całkowitą
    }
    int i = 0;
    if (data[0] == '-' || data[0] == '+') {
        i++; // ignoruj ewentualny znak minusa lub plusa na początku
    }
    for (; data[i] != '\0'; i++) {
        if (data[i] > '9') {
            return false; // znaleziono znak niebędący cyfrą
        }
    }
    return true;
}

int String::length() const {
    int len = 0;
    while (data[len] != '\0') {
        len++;
    }
    return len;
}


char* String::extractChar(int index) const {
    char* ptr = &data[index];
    return ptr;
}

int String::convertStrToInt(const char *data) {
    int num = 0;
    for (int i = 0; i < strlen(data); i++, num *= 10) {
        num += data[i] - '0';
    }
    return num / 10;
}

String& String::strcpy(String str) {
    // Usuwamy zawartość bieżącego obiektu
    clear();

    // Kopia zawartości str do bieżącego obiektu
    for (int i = 0; str[i] != '\0'; i++) {
        pushBack(str[i]);
    }

    return *this;
}

String& String::strcat(const String& other) {
    int len1 = length();
    int len2 = other.length();
    char* newStr = new char[len1 + len2 + 1];
    ::strcpy(newStr, data);
    ::strcpy(newStr + len1, other.data);
    delete[] data;
    data = newStr;
    return *this;
}

String* String::strtok(char delim) {
    String* token = new String();
    int i = 0;
    while (data[i] != '\0') {
        if (data[i] == delim) {
            i++;
            break;
        }
        token->pushBack(data[i]);
        i++;
    }
    if (data[i] == '\0' && token->empty()) {
        delete token;
        return nullptr;
    }
    return token;
}

