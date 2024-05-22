#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>


class String {

public:
    // Konstruktor
    String();
    // Konstruktor kopiujący
    String(const String& other);
    // Destruktor
    ~String();
    // Przypisanie
    String& operator=(const String& other);
    String& operator=(const char* str);
    // Dodawanie
    String operator+(const String& other) const;
    String& operator+=(const String& other);
    String& operator+=(const char other);
    // Porównanie
    bool operator==(const String& other) const;
    bool operator==(const char* s) const;
    // Zwraca długość
    int getLength() const;
    // Zwraca znak na danej pozycji
    char operator[](int index) const;
    // Dodaje znak na koniec
    void pushBack(char c);
    // Czyści stringa
    void clear();
    // Zwraca wskaźnik na dane
    const char* c_str() const;
    String convertCharArrayToString(char charArray[]);
    String substring(int start, int length);
    int find(char c) const;
    int count(char c) const;
    char getChar(int index) const;
    //String cut(int start, int length) const;
    bool empty() const;
    void erase(int position, int count);
    void trim(String& data);
    int my_strlen(const char *str);
    bool checkInt(char* data);
    int length() const;
    char* extractChar(int index) const;
    int convertStrToInt(const char* data);
    String& strcpy(String str);
    String& strcat(const String& other);
    String* strtok(char delim);

    friend std::istream& operator>>(std::istream& is, String& str) {
        char buffer[1024];
        is.getline(buffer, 1024);
        str = buffer;
        return is;
    }


private:
    char* data;
    int size;
};