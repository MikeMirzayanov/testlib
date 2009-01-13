#ifndef _TESTLIB_H_
#define _TESTLIB_H_

/*
 *
 * Copyright (c) 2005-2008
 */

#define VERSION "0.5.2"

/* 
 * Mike Mirzayanov
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/* NOTE: This file contains testlib library for C++.
 *
 *   Program, using testlib running format:
 *     check.exe <Input_File> <Output_File> <Answer_File> [<Result_File> [-appes]],
 *
 *   If result file is specified it will contain results.
 */

const char* latestFeatures[] = {
                          "Added InStream::readLong() and removed InStream::readLongint()",
                          "Now no footer added to each report by default (use directive FOOTER to switch on)",
                          "Now every checker has a name, use setName(const char* format, ...) to set it",
                          "Now it is compatible with TTS (by Kittens Computing)",
                          "Added \'ensure(condition, message = \"\")\' feature, it works like assert()",
                          "Fixed compatibility with MS C++ 7.1",
                          "Added footer with exit code information",
                          "Added compatibility with EJUDGE (compile with EJUDGE directive)"
                         };

#include <cstdio>
#include <cctype>
#include <string>
#include <string.h>
#include <stdarg.h>
#include <cstdlib>

#if ( _WIN32 || __WIN32__ )
#include <windows.h>
#else
#define WORD unsigned short
#endif

#define ABS(f) ((f) < 0 ? -(f) : (f))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define OUTPUT_BUFFER_SIZE (2097152)

#define LF (char)10
#define CR (char)13
#define TAB (char)9
#define SPACE (char)' '

#define EOFCHAR (EOF)
#define EOFREMAP SPACE
#define NUMBERBEFORE LF, CR, SPACE, TAB, EOFCHAR
#define NUMBERAFTER LF, CR, TAB, EOFCHAR
#define LINEAFTER LF, CR, EOFCHAR
#define BLANKS LF, CR, SPACE, TAB
#define EOLNCHAR LF, CR, EOFCHAR

#ifndef EJUDGE
#define OK_EXIT_CODE 0
#define WA_EXIT_CODE 1
#define PE_EXIT_CODE 2
#define FAIL_EXIT_CODE 3
#define DIRT_EXIT_CODE 4
#else
#define OK_EXIT_CODE 0
#define WA_EXIT_CODE 5
#define PE_EXIT_CODE 4
#define FAIL_EXIT_CODE 6
#define DIRT_EXIT_CODE 6
#endif

inline bool isEofChar(char c)
{
    return (c == EOFCHAR);
}

inline bool isEofRemap(char c)
{
    return (c == EOFREMAP);
}

inline bool isNumberBefore(char c)
{
    return (c == LF || c == CR || c == SPACE || c == TAB);
}

inline bool isNumberAfter(char c)
{
    return (c == LF || c == CR || c == SPACE || c == TAB || c == EOFCHAR || c == (char)26);
}

inline bool isLineAfter(char c)
{
    return (c == LF || c == CR || c == EOFCHAR || c == (char)26);
}

inline bool isBlanks(char c)
{
    return (c == LF || c == CR || c == SPACE || c == TAB);
}

inline bool isEolnChar(char c)
{
    return (c == LF || c == CR || c == EOFCHAR || c == (char)26);
}

struct TCharSet
{
    unsigned int data[64];
    
    void insert(char c)
    {
        int pc = (int)c;
        data[pc >> 3] |= (1 << (pc & 7));
    }

    bool count(char c)
    {
        unsigned int pc = (unsigned char)c;
        return (data[pc >> 3] & (1 << (pc & 7))) != 0;
    }

    void clear()
    {
        memset(data, 0, sizeof(data));
    }

    TCharSet()
    {
        clear();
    }

    TCharSet(char c0)
    {
        clear();
        insert(c0);
    }

    TCharSet(char c0, char c1)
    {
        clear();
        insert(c0); insert(c1);
    }

    TCharSet(char c0, char c1, char c2)
    {
        clear();
        insert(c0); insert(c1); insert(c2);
    }

    TCharSet(char c0, char c1, char c2, char c3)
    {
        clear();
        insert(c0); insert(c1); insert(c2); insert(c3);
    }

    TCharSet(char c0, char c1, char c2, char c3, char c4)
    {
        clear();
        insert(c0); insert(c1); insert(c2); insert(c3); insert(c4);
    }
};

enum TMode
{
    _input, _output, _answer
};

enum TResult
{
    _ok, _wa, _pe, _fail, _dirt
};

const std::string outcomes[] =
    {"accepted", "wrong-answer", "presentation-error", "fail", "fail"};

struct InStream
{
    InStream();

    std::FILE * file;
    std::string name;
    TMode mode;
    bool opened;
    void init(std::string fileName, TMode mode);
    
    char curChar();
    void skipChar();
    char nextChar();

    void reset();
    bool eof();
    bool seekEof();

    bool eoln();
    bool seekEoln();

    void nextLine();
    void skip(TCharSet setof);
    std::string readWord(TCharSet before, TCharSet after);
    std::string readWord();

    long long readLong();
    int readInteger();
    int readInt();

    double readReal();
    double readDouble();
    
    std::string readString();

    void quit(TResult result, const char * msg);
    void quits(TResult result, std::string msg);

    void close();

    const static WORD LightGray = 0x07;    
    const static WORD LightRed = 0x0c;    
    const static WORD LightCyan = 0x0b;    
    const static WORD LightGreen = 0x0a;    

    static void textColor(WORD color);
    static void quitscr(WORD color, const char * msg);
    static void quitscrS(WORD color, std::string msg);
    void xmlSafeWrite(std::FILE * file, const char * msg);
};

InStream inf;
InStream ouf;
InStream ans;
bool appesMode;
std::string resultName;
std::string checkerName = "untitled checker";

/* implementation
 */

InStream::InStream()
{
    file = NULL;
    name = "";
    mode = _input;
}

int resultExitCode(TResult r)
{
    if (r == _ok)
        return OK_EXIT_CODE;
    if (r == _wa)
        return WA_EXIT_CODE;
    if (r == _pe)
        return PE_EXIT_CODE;
    if (r == _fail)
        return FAIL_EXIT_CODE;
    if (r == _dirt)
        return DIRT_EXIT_CODE;
    return FAIL_EXIT_CODE;
}

void InStream::textColor(WORD color)
{
#if ( _WIN32 || __WIN32__ )
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, color);
#endif
}

void halt(int exitCode)
{
#ifdef FOOTER
    InStream::textColor(InStream::LightGray);
    std::printf("Checker: \"%s\"\n", checkerName.c_str());
    std::printf("Exit code: %d\n", exitCode);
    InStream::textColor(InStream::LightGray);
#endif
    std::exit(exitCode);
}

void InStream::quit(TResult result, const char * msg)
{
    if (mode != _output && result != _fail)
        quits(_fail, std::string(msg) + " (" + name + ")");

    std::FILE * resultFile;
    std::string errorName;

    if (result == _ok)
    {
        if (!ouf.seekEof())
            quit(_dirt, "Extra information in the output file");
    }

    switch (result)
    {
    case _fail:
        errorName = "FAIL ";
        quitscrS(LightRed, errorName);
        break;
    case _dirt:
        errorName = "wrong output format ";
        quitscrS(LightCyan, errorName);
        result = _pe;
        break;
    case _pe:
        errorName = "wrong output format ";
        quitscrS(LightRed, errorName);
        break;
    case _ok:
        errorName = "ok ";
        quitscrS(LightGreen, errorName);
        break;
    case _wa:
        errorName = "wrong answer ";
        quitscrS(LightRed, errorName);
        break;
    default:
        quit(_fail, "What is the code ??? ");
    }

    if (resultName != "")
    {
        resultFile = std::fopen(resultName.c_str(), "w");
        if (resultFile == NULL)
            quit(_fail, "Can not write to Result file");
        if (appesMode)
        {
            fprintf(resultFile, "<?xml version=\"1.0\" encoding=\"windows-1251\"?>");
            fprintf(resultFile, "<result outcome = \"%s\">", outcomes[(int)result].c_str());
            xmlSafeWrite(resultFile, msg);
            fprintf(resultFile, "</result>\n");
        }
        else
        {
            /** old-style format
             *   fprintf(resultFile, ".Testlib Result Number = %d\n", (int)result);
             *   fprintf(resultFile, ".Result name (optional) = %s\n", errorName.c_str());
             *   fprintf(resultFile, ".Check Comments = %s\n", msg);
             */
             fprintf(resultFile, "%s", msg);
        }
        if (NULL == resultFile || fclose(resultFile) != 0)
            quit(_fail, "Can not write to Result file");
    }

    quitscr(LightGray, msg);
    std::printf("\n");

    if (inf.file)
        fclose(inf.file);
    if (ouf.file)
        fclose(ouf.file);
    if (ans.file)
        fclose(ans.file);

    textColor(LightGray);

    if (resultName != "")
        std::printf("See file to check exit message\n");

    halt(resultExitCode(result));
}

void InStream::quits(TResult result, std::string msg)
{
    InStream::quit(result, msg.c_str());
}

void InStream::xmlSafeWrite(std::FILE * file, const char * msg)
{
    size_t lmsg = strlen(msg);
    for (size_t i = 0; i < lmsg; i++)
    {
        if (msg[i] == '&')
        {
            fprintf(file, "%s", "&amp;");
            continue;
        }
        if (msg[i] == '<')
        {
            fprintf(file, "%s", "&lt;");
            continue;
        }
        if (msg[i] == '>')
        {
            fprintf(file, "%s", "&gt;");
            continue;
        }
        if (msg[i] == '"')
        {
            fprintf(file, "%s", "&quot;");
            continue;
        }
        if (0 <= msg[i] && msg[i] <= 31)
        {
            fprintf(file, "%c", '.');
            continue;
        }
        fprintf(file, "%c", msg[i]);
    }
}

void InStream::quitscrS(WORD color, std::string msg)
{
    quitscr(color, msg.c_str());
}

void InStream::quitscr(WORD color, const char * msg)
{
    if (resultName == "")
    {
        textColor(color);
        std::printf("%s", msg);
        textColor(LightGray);
    }
}

void InStream::reset()
{
    if (opened)
        close();

    if (NULL == (file = std::fopen(name.c_str(), "r")))
    {
        if (mode == _output)
            quits(_pe, std::string("File not found: \"") + name + "\"");
    }

    opened = true;
}

void InStream::init(std::string fileName, TMode mode)
{
    opened = false;
    name = fileName;
    this->mode = mode;
    reset();
}

char InStream::curChar()
{
    char c = (char)getc(file);
    ungetc(c, file);
    return c;
}

char InStream::nextChar()
{
    return (char)getc(file);
}

void InStream::skipChar()
{
    getc(file);
}

std::string InStream::readWord(TCharSet before, TCharSet after)
{
    char cur;
    while (before.count(cur = (char)getc(file)) == 1);
    if (cur == EOFCHAR && !after.count(cur))
    {
        quit(_pe, "Unexpected end of file");
    }
    std::string result = "";
    while (!(after.count(cur) || cur == EOFCHAR))
    {
        result += cur;
        cur = (char)getc(file);
    }
    ungetc(cur, file);

    return result;
}

std::string InStream::readWord()
{
    return readWord(TCharSet(BLANKS), TCharSet(BLANKS));
}

int InStream::readInteger()
{
    char cur;
    while (isNumberBefore(cur = (char)getc(file)));
    if (cur == EOFCHAR)
        quit(_pe, "Unexpected end of file - integer expected");
    ungetc(cur, file);
    int retval;
    if (fscanf(file, "%d", &retval) != 1)
        // todo: show insted-of
        quit(_pe, "Expected integer");
    return retval;
}

long long InStream::readLong()
{
    char cur;
    while (isNumberBefore(cur = (char)getc(file)));
    if (cur == EOFCHAR)
        quit(_pe, "Unexpected end of file - long expected");
    ungetc(cur, file);
    long long retval;
    if (fscanf(file, "%I64d", &retval) != 1)
        // todo: show insted-of
        quit(_pe, "Expected long");
    return retval;
}

int InStream::readInt()
{
    return readInteger();
}

double InStream::readReal()
{
    if (seekEof())
        quit(_pe, "Unexpected end of file - double expected");

    double retval;
    if (fscanf(file, "%lf", &retval) != 1)
        // todo: show insted-of
        quit(_pe, "Expected double");
    return retval;
}

double InStream::readDouble()
{
    return readReal();
}

void InStream::skip(TCharSet setof)
{
    char cur;
    while (setof.count(cur = (char)getc(file)) == 1);
    ungetc(cur, file);
}

bool InStream::eof()
{
    return (NULL == file || feof(file) != 0);
}

bool InStream::seekEof()
{
    if (NULL == file)
        return true;
    char cur;
    while (isBlanks(cur = (char)getc(file)));
    ungetc(cur, file);
    return (NULL == file || feof(file) != 0 || cur == EOF);
}

bool InStream::eoln()
{
    if (NULL == file)
        return true;
    char c = curChar();
    return isEolnChar(c);
}

bool InStream::seekEoln()
{
    if (NULL == file)
        return true;
    char cur;
    do
    {
        cur = (char)getc(file);
    } 
    while (cur == SPACE || cur == TAB);
    ungetc(cur, file);
    return isEolnChar(cur);
}

void InStream::nextLine()
{
    if (NULL == file)
        return;
    char cur;
    while (!isEolnChar(cur = (char)getc(file)));
    if (cur == CR)
    {
        cur = (char)getc(file);
        if (cur != LF)
            ungetc(cur, file);
    }
    else
    {
        if (cur != LF)
            ungetc(cur, file);
    }
}

std::string InStream::readString()
{
    if (NULL == file)
        quit(_pe, "Expected line");

    std::string retval = "";
    char cur;
    while (!isEolnChar(cur = (char)getc(file)))
        retval += cur;
    if (cur == CR)
    {
        cur = (char)getc(file);
        if (cur != LF)
            ungetc(cur, file);
    }
    else
    {
        if (cur != LF)
            ungetc(cur, file);
    }
    
    return retval;
}

void InStream::close()
{
    if (opened)
        fclose(file);
    opened = false;
}

void quit(TResult result, const std::string& msg)
{
    ouf.quit(result, msg.c_str());
}

void quit(TResult result, const char * msg)
{
    ouf.quit(result, msg);
}

void quitf(TResult result, const char * format, ...)
{
    char * buffer = new char [OUTPUT_BUFFER_SIZE];
    
    va_list ap;
    va_start(ap, format);
    std::vsprintf(buffer, format, ap);
    va_end(ap);

    std::string output(buffer);
    delete[] buffer;

    quit(result, output);
}

void registerTestlibCmd(int argc, char * argv[])
{
    if (argc > 1 && !strcmp("--help", argv[1]))
    {
        InStream::textColor(InStream::LightCyan);
        std::printf("TESTLIB %s ", VERSION);
        std::printf("by Mike Mirzayanov, copyright(c) 2005-2006\n");
        std::printf("Checker name: \"%s\"\n", checkerName.c_str());
        InStream::textColor(InStream::LightGray);

        std::printf("\n");
        std::printf("Latest features: \n");
        for (size_t i = 0; i < sizeof(latestFeatures) / sizeof(char*); i++)
        {
            std::printf("*) %s\n", latestFeatures[i]);
        }
        std::printf("\n");

        std::printf("Program must be run with the following arguments: \n");
        std::printf("    <input-file> <output-file> <answer-file> [<report-file> [<-appes>]]\n\n");

        std::exit(0);
    }

    if (sizeof(int) != 4)
        quit(_fail, "'testlib' unit assumes 'sizeof(integer) = 4'");

    if (argc  < 4 || argc > 6)
    {
        quit(_fail, std::string("Program must be run with the following arguments: ") +
            std::string("<input-file> <output-file> <answer-file> [<report-file> [<-appes>]]") + 
            "\nUse \"--help\" to get help information");
    }

    if (argc == 4)
    {
        resultName = "";
        appesMode = false;
    }

    if (argc == 5)
    {
        resultName = argv[4];
        appesMode = false;
    }

    if (argc == 6)
    {
        if (strcmp("-APPES", argv[5]) && strcmp("-appes", argv[5]))
        {
            quit(_fail, std::string("Program must be run with the following arguments: ") +
                        "<input-file> <output-file> <answer-file> [<report-file> [<-appes>]]");
        }
        else
        {
            resultName = argv[4];
            appesMode = true;
        }
    }

    inf.init(argv[1], _input);
    ouf.init(argv[2], _output);
    ans.init(argv[3], _answer);
}

void registerTestlib(int argc, ...)
{
    if (argc  < 3 || argc > 5)
        quit(_fail, std::string("Program must be run with the following arguments: ") +
            "<input-file> <output-file> <answer-file> [<report-file> [<-appes>]]");

    char ** argv = new char*[argc + 1];
    
    va_list ap;
    va_start(ap, argc);
    argv[0] = NULL;
    for (int i = 0; i < argc; i++)
    {
        argv[i + 1] = va_arg(ap, char *);
    }
    va_end(ap);

    registerTestlibCmd(argc + 1, argv);
    delete[] argv;
}

inline bool isNaN(double r)
{
    return ((r != r) == true) && ((r == r) == false) && ((1.0 > r) == false) && ((1.0 < r) == false);
}

inline bool isInfinite(double r)
{
    return (r > 1E100 || r < -1E100);
}

bool doubleCompare(double expected, double result, double MAX_DOUBLE_ERROR)
{
        if(isNaN(expected))
        {
            return isNaN(result);
        }
        else 
            if(isInfinite(expected))
            {
                if(expected > 0)
                {
                    return result > 0 && isInfinite(result);
                }
                else
                {
                    return result < 0 && isInfinite(result);
                }
            }
            else 
                if(isNaN(result) || isInfinite(result))
                {
                    return false;
                }
                else 
                if(ABS(result - expected) < MAX_DOUBLE_ERROR)
                {
                    return true;
                }
                else
                {
                    double minv = MIN(expected * (1.0 - MAX_DOUBLE_ERROR),
                                 expected * (1.0 + MAX_DOUBLE_ERROR));
                    double maxv = MAX(expected * (1.0 - MAX_DOUBLE_ERROR),
                                  expected * (1.0 + MAX_DOUBLE_ERROR));
                    return result > minv && result < maxv;
                }
}

double doubleDelta(double expected, double result)
{
    double absolute = ABS(result - expected);
    
    if (ABS(expected) > 1E-9)
    {
        double relative = ABS(absolute / expected);
        return MIN(absolute, relative);
    }
    else
        return absolute;
}

void ensure(bool cond, const std::string msg = "")
{
    if (!cond)
    {
        quitf(_fail, msg.c_str());
    }
}

void setName(const char* format, ...)
{
    char * buffer = new char [OUTPUT_BUFFER_SIZE];
    
    va_list ap;
    va_start(ap, format);
    std::vsprintf(buffer, format, ap);
    va_end(ap);

    std::string name(buffer);
    delete[] buffer;

    checkerName = name;
}

#endif
