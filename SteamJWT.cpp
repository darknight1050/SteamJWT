#pragma comment(lib, "crypt32.lib")

#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <process.h>

void MyHandleError(char* s);

std::string hexStr(const uint8_t* data, int len)
{
    std::stringstream ss;
    ss << std::hex;

    for (int i(0); i < len; ++i)
        ss << std::setw(2) << std::setfill('0') << (int)data[i];

    return ss.str();
}

int char2int(char input)
{
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw "Invalid input string";
}

void hex2bin(const char* src, char* target)
{
    while (*src && src[1])
    {
        *(target++) = char2int(*src) * 16 + char2int(src[1]);
        src += 2;
    }
}

void Decrypt(DATA_BLOB* DataEntropy, DATA_BLOB* DataIn) {
    LPWSTR pDescrOut = NULL;
    DATA_BLOB DataOut;
    if (CryptUnprotectData(
        DataIn,
        &pDescrOut,
        DataEntropy,
        NULL,
        NULL,
        CRYPTPROTECT_UI_FORBIDDEN,
        &DataOut))
    {
        printf("Result: %s\n", hexStr(DataOut.pbData, DataOut.cbData).c_str());
        LocalFree(DataOut.pbData);
        LocalFree(pDescrOut);
    }
    else
    {
        MyHandleError((char*)"Decryption error!");
    }
}

void Encrypt(DATA_BLOB* DataEntropy, DATA_BLOB* DataIn) {
    DATA_BLOB DataOut;
    if (CryptProtectData(
        DataIn,
        L"BObfuscateBuffer",
        DataEntropy,
        NULL,
        NULL,
        CRYPTPROTECT_UI_FORBIDDEN,
        &DataOut))
    {
        printf("Result: %s\n", hexStr(DataOut.pbData, DataOut.cbData).c_str());
        LocalFree(DataOut.pbData);
    }
    else
    {
        MyHandleError((char*)"Decryption error!");
    }
}

void main()
{

    printf("1 - Decrypt\n2 - Encrypt\nChoose: ");
    int choose;
    scanf_s("%d", &choose, 1);

    if (choose < 1 || choose > 2)
    {
        printf("Invalid Option\n");
        system("pause");
        exit(0);
    }

    printf("Name: ");
    char name[1024];
    scanf_s("%s", &name, (unsigned)_countof(name));

    DATA_BLOB DataEntropy;
    DataEntropy.pbData = (BYTE*)name;
    DataEntropy.cbData = strlen(name);


    printf("Data: ");
    char data[4 * 1024];
    scanf_s("%s", &data, (unsigned)_countof(data));

    size_t len = strlen(data);
    BYTE* binary = (BYTE*)LocalAlloc(0, len / 2);
    hex2bin(data, (char*)binary);
    DATA_BLOB DataIn;
    DataIn.pbData = (BYTE*)binary;
    DataIn.cbData = len / 2;

    switch (choose) {
        case 1:
            Decrypt(&DataEntropy, &DataIn);
            break;
        case 2:
            Encrypt(&DataEntropy, &DataIn);
    }


    LocalFree(binary);
    system("pause");
}

void MyHandleError(char* s)
{
    fprintf(stderr, "An error occurred in running the program. \n");
    fprintf(stderr, "%s\n", s);
    fprintf(stderr, "Error number %x.\n", GetLastError());
    fprintf(stderr, "Program terminating. \n");
    system("pause");
    exit(1);
}