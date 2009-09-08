/* 
 * It is strictly recommended to include "testlib.h" before any other include 
 * in your code. In this case testlib overrides compiler specific "random()".
 *
 * If you can't compile your code and compiler outputs something about 
 * ambiguous call of "random_shuffle", "rand" or "srand" it means that 
 * you shouldn't use them. Use "shuffle", and "rnd.next()" instead of them
 * because these calls produce stable result for any C++ compiler. Read 
 * sample generator sources for clarification.
 *
 * Please read the documentation for class "random" and use "rnd" instance in
 * generators. Probably, these sample calls will be usefull for you:
 *              rnd.next(); rnd.next(100); rnd.next(1, 2); 
 *              rnd.next(3.14); rnd.next("[a-z]{1,100}").
 *
 * Also read about wnext() to generate off-center random distribution.
 */

#ifndef _TESTLIB_H_
#define _TESTLIB_H_

/*
 * Copyright (c) 2005-2009                                
 */

#define VERSION "0.6.2"

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
 *   Check, using testlib running format:
 *     check.exe <Input_File> <Output_File> <Answer_File> [<Result_File> [-appes]],
 *   If result file is specified it will contain results.
 *
 *   Validator, using testlib running format:                                          
 *     validator.exe < input.txt,
 *   It will return non-zero exit code and writes message to standard output.
 *
 *   Generator, using testlib running format:                                          
 *     gen.exe [parameter-1] [parameter-2] [... paramerter-n],
 *   You can write generated test(s) into standard output or into the file(s).
 */

const char* latestFeatures[] = {
                          "Supported \"partially correct\", example: quitf(_pc(13), \"result=%d\", result)",  
                          "Added shuffle(begin, end), use it instead of random_shuffle(begin, end)",  
                          "Added readLine(const string& ptrn), fixed the logic of readLine() in the validation mode",  
                          "Package extended with samples of generators and validators",  
                          "Written the documentation for classes and public methods in testlib.h",
                          "Implemented random routine to support generators, use registerGen() to switch it on",
                          "Implemented strict mode to validate tests, use registerValidation() to switch it on",
                          "Now ncmp.cpp and wcmp.cpp are return WA if answer is suffix or prefix of the output",
                          "Added InStream::readLong() and removed InStream::readLongint()",
                          "Now no footer added to each report by default (use directive FOOTER to switch on)",
                          "Now every checker has a name, use setName(const char* format, ...) to set it",
                          "Now it is compatible with TTS (by Kittens Computing)",
                          "Added \'ensure(condition, message = \"\")\' feature, it works like assert()",
                          "Fixed compatibility with MS C++ 7.1",
                          "Added footer with exit code information",
                          "Added compatibility with EJUDGE (compile with EJUDGE directive)"
                         };

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

/* Overrides random() for Borland C++. */
#define random __random_deprecated
#include <stdlib.h>
#include <cstdlib>
#include <climits>
#include <algorithm>
#undef random

#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>
#include <cstring>
#include <stdarg.h>

#include <fcntl.h>

#if !defined(unix)
#include <io.h>
#endif

#if ( _WIN32 || __WIN32__ || _WIN64 || __WIN64__ )
#include <windows.h>
#define ON_WINDOWS
#else
#define WORD unsigned short
#endif

#ifndef LLONG_MIN
#define LLONG_MIN   (-9223372036854775807LL - 1)
#endif

#define MAX_FORMAT_BUFFER_SIZE (2097152)

#define LF ((char)10)
#define CR ((char)13)
#define TAB ((char)9)
#define SPACE ((char)' ')
#define EOFC ((char)26)

#ifndef EJUDGE
#define OK_EXIT_CODE 0
#define WA_EXIT_CODE 1
#define PE_EXIT_CODE 2
#define FAIL_EXIT_CODE 3
#define DIRT_EXIT_CODE 4
#define PC_BASE_EXIT_CODE 0
#else
#define OK_EXIT_CODE 0
#define WA_EXIT_CODE 5
#define PE_EXIT_CODE 4
#define FAIL_EXIT_CODE 6
#define DIRT_EXIT_CODE 6
#define PC_BASE_EXIT_CODE 0
#endif

#define __TESTLIB_STATIC_ASSERT(condition) typedef void* __testlib_static_assert_type[((condition) != 0) * 2 - 1];

template<typename T>
static inline T __testlib_abs(const T& x)
{
    return x > 0 ? x : -x;
}

template<typename T>
static inline T __testlib_min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template<typename T>
static inline T __testlib_max(const T& a, const T& b)
{
    return a > b ? a : b;
}

/*
 * Very simple regex-like pattern.
 * It used for two purposes: validation and generation.
 * 
 * For example, pattern("[a-z]{1,5}").next(rnd) will return
 * random string from lowercase latin letters with length 
 * from 1 to 5. It is easier to call rnd.next("[a-z]{1,5}") 
 * for the same effect. 
 * 
 * Another samples:
 * "mike|john" will generate (match) "mike" or "john";
 * "-?[1-9][0-9]{0,3}" will generate (match) non-zero integers from -9999 to 9999;
 * "id-([ac]|b{2})" will generate (match) "id-a", "id-bb", "id-c";
 * "[^0-9]*" will match sequences (empty or non-empty) without digits, you can't 
 * use it for generations.
 *
 * You can't use pattern for generation if it contains meta-symbol '*'. Also it
 * is not recommended to use it for char-sets with meta-symbol '^' like [^a-z].
 *
 * For matching very simple greedy algorithm is used. For example, pattern
 * "[0-9]?1" will not match "1", because of greedy nature of matching.
 * Alternations (meta-symbols "|") are processed with brute-force algorithm, so 
 * do not use many alternations in one expression.
 *
 * If you want to use one expression many times it is better to compile it into
 * a single pattern like "pattern p("[a-z]+")". Later you can use 
 * "p.matches(std::string s)" or "p.next(random& rd)" to check matching or generate
 * new string by pattern.
 * 
 * Simpler way to read token and check it for pattern matching is "inf.readToken("[a-z]+")".
 */
class random;

class pattern
{
public:
    /* Create pattern instance by string. */
    pattern(std::string s);
    /* Generate new string by pattern and given random. */
    std::string next(random& rnd) const;
    /* Checks if given string match the pattern. */
    bool matches(const std::string& s) const;

private:
    bool matches(const std::string& s, size_t pos) const;

    std::vector<pattern> children;
    std::vector<char> chars;
    int from;
    int to;
};

/* 
 * Use random instances to generate random values. It is preffered
 * way to use randoms instead of rand() function or self-written 
 * randoms.
 *
 * Testlib defines global variable "rnd" of random class.
 * Use registerGen(argc, argv) to setup random seed be command
 * line.
 *
 * Random generates uniformly distributed values if another strategy is
 * not specified explicitly.
 */
class random
{
private:
    long long seed;
    static const long long multiplier;
    static const long long addend;
    static const long long mask;
    static const int lim;
    
    long long nextBits(int bits) 
    {
        if (bits <= 48)
        {
            seed = (seed * multiplier + addend) & mask;
            return (long long)(seed >> (48 - bits));
        }
        else
        {
            if (bits > 63)
                throw "n must be less than 64";

            return ((nextBits(31) << 32) ^ nextBits(31));
        }
    }

public:
    /* New random with fixed seed. */
    random()
        : seed(3905348978240129619LL)
    {
    }

    /* Sets seed by command line. */
    void setSeed(int argc, char* argv[])
    {
        random p;

        seed = 3905348978240129619LL;
        for (int i = 1; i < argc; i++)
        {
            std::size_t le = std::strlen(argv[i]);
            for (std::size_t j = 0; j < le; j++)
                seed = seed * multiplier * (unsigned int)(argv[i][j]) + addend;
            seed += multiplier / addend;
        }

        seed = seed & mask;
    }

    /* Sets seed by given value. */ 
    void setSeed(long long _seed)
    {
        _seed = (_seed ^ multiplier) & mask;
        seed = _seed;
    }

    /* Random value in range [0, n-1]. */
    int next(int n) 
    {
        if (n <= 0)
            throw "n must be positive";

        if ((n & -n) == n)  // n is a power of 2
            return (int)((n * (long long)nextBits(31)) >> 31);

        int bits, val;
        
        do 
        {
            bits = int(nextBits(31));
            val = bits % n;
        } while (bits - val + (n - 1) < 0);

        return val;
    }

    /* Random value in range [0, n-1]. */
    int next(unsigned int n)
    {
        if (n >= INT_MAX)
            throw "n must be less INT_MAX";
        return next(int(n));
    }

    /* Random value in range [0, n-1]. */
    long long next(long long n) 
    {
        if (n <= 0)
            throw "n must be positive";

        long long bits, val;
        
        do 
        {
            bits = nextBits(63);
            val = bits % n;
        } while (bits - val + (n - 1) < 0);

        return val;
    }

    /* Returns random value in range [from,to]. */
    int next(int from, int to)
    {
        return next(to - from + 1) + from;
    }

    /* Returns random value in range [from,to]. */
    int next(unsigned int from, unsigned int to)
    {
        return next(to - from + 1) + from;
    }

    /* Returns random value in range [from,to]. */
    long long next(long long from, long long to)
    {
        return next(to - from + 1) + from;
    }

    /* Random double value in range [0, 1). */
    double next() 
    {
        return (((long long)(nextBits(26)) << 27) + nextBits(27)) / (double)(1LL << 53);
    }

    /* Random double value in range [0, n). */
    double next(double n)
    {
        return n * next();
    }

    /* Random double value in range [from, to). */
    double next(double from, double to)
    {
        return next(to - from) + from;
    }

    /* Random string value by given pattern (see pattern documentation). */
    std::string next(const std::string& ptrn)
    {
        pattern p(ptrn);
        return p.next(*this);
    }

    /* Random string value by given pattern (see pattern documentation). */
    std::string next(const char* format, ...)
    {
        char* buffer = new char [MAX_FORMAT_BUFFER_SIZE];
        
        va_list ap;
        va_start(ap, format);
        std::vsprintf(buffer, format, ap);
        va_end(ap);

        std::string ptrn(buffer);
        delete[] buffer;

        return next(ptrn);
    }

    /* 
     * Weighted next. If type == 0 than it is usual "next()".
     *
     * If type = 1, than it returns "max(next(), next())"
     * (the number of "max" functions equals to "type").
     *
     * If type < 0, than "max" function replaces with "min".
     */
    int wnext(int n, int type)
    {
        if (n <= 0)
            throw "n must be positive";
        
        if (abs(type) < random::lim)
        {
            int result = next(n);

            for (int i = 0; i < +type; i++)
                result = __testlib_max(result, next(n));
            
            for (int i = 0; i < -type; i++)
                result = __testlib_min(result, next(n));

            return result;
        }
        else
        {
            double p;
            
            if (type > 0)
                p = std::pow(next() + 0.0, 1.0 / (type + 1));
            else
                p = 1 - std::pow(next() + 0.0, 1.0 / (-type + 1));

            return int(n * p);
        }
    }
    
    /* See wnext(int, int). It uses the same algorithms. */
    int wnext(unsigned int n, int type)
    {
        if (n >= INT_MAX)
            throw "n must be less INT_MAX";
        return wnext(int(n), type);
    }
    
    /* See wnext(int, int). It uses the same algorithms. */
    long long wnext(long long n, int type)
    {
        if (n <= 0)
            throw "n must be positive";
        
        if (abs(type) < random::lim)
        {
            long long result = next(n);

            for (int i = 0; i < +type; i++)
                result = __testlib_max(result, next(n));
            
            for (int i = 0; i < -type; i++)
                result = __testlib_min(result, next(n));

            return result;
        }
        else
        {
            double p;
            
            if (type > 0)
                p = std::pow(next() + 0.0, 1.0 / (type + 1));
            else
                p = std::pow(next() + 0.0, - type + 1);

            return (long long)(n * p);
        }
    }
    
    /* See wnext(int, int). It uses the same algorithms. */
    double wnext(int type)
    {
        if (abs(type) < random::lim)
        {
            double result = next();

            for (int i = 0; i < +type; i++)
                result = __testlib_max(result, next());
            
            for (int i = 0; i < -type; i++)
                result = __testlib_min(result, next());

            return result;
        }
        else
        {
            double p;
            
            if (type > 0)
                p = std::pow(next() + 0.0, 1.0 / (type + 1));
            else
                p = std::pow(next() + 0.0, - type + 1);

            return p;
        }
    }
    
    /* See wnext(int, int). It uses the same algorithms. */
    double wnext(double n, int type)
    {
        if (n <= 0)
            throw "n must be positive";

        if (abs(type) < random::lim)
        {
            double result = next();

            for (int i = 0; i < +type; i++)
                result = __testlib_max(result, next());
            
            for (int i = 0; i < -type; i++)
                result = __testlib_min(result, next());

            return n * result;
        }
        else
        {
            double p;
            
            if (type > 0)
                p = std::pow(next() + 0.0, 1.0 / (type + 1));
            else
                p = std::pow(next() + 0.0, - type + 1);

            return n * p;
        }
    }

    /* Returns weighted random value in range [from, to]. */
    int wnext(int from, int to, int type)
    {
        return wnext(to - from + 1, type) + from;
    }
    
    /* Returns weighted random value in range [from, to]. */
    int wnext(unsigned int from, unsigned int to, int type)
    {
        return wnext(to - from + 1, type) + from;
    }
    
    /* Returns weighted random value in range [from, to]. */
    long long wnext(long long from, long long to, int type)
    {
        return wnext(to - from + 1, type) + from;
    }
    
    /* Returns weighted random double value in range [from, to). */
    double wnext(double from, double to, int type)
    {
        return wnext(to - from, type) + from;
    }
};

const int random::lim = 25;
const long long random::multiplier = 0x5DEECE66DLL;
const long long random::addend = 0xBLL;
const long long random::mask = (1LL << 48) - 1;

/* Pattern implementation */
bool pattern::matches(const std::string& s) const
{
    return matches(s, 0);
}

static bool __pattern_isSlash(const std::string& s, size_t pos)
{
    return s[pos] == '\\';
}

static bool __pattern_isCommandChar(const std::string& s, size_t pos, char value)
{
    if (pos >= s.length())
        return false;

    int slashes = 0;

    int before = pos - 1;
    while (before >= 0 && s[before] == '\\')
        before--, slashes++;

    return slashes % 2 == 0 && s[pos] == value;
}

static char __pattern_getChar(const std::string& s, size_t& pos)
{
    if (__pattern_isSlash(s, pos))
        pos += 2;
    else
        pos++;

    return s[pos - 1];
}

static int __pattern_greedyMatch(const std::string& s, size_t pos, const std::vector<char> chars)
{
    int result = 0;

    while (pos < s.length())
    {
        char c = __pattern_getChar(s, pos);
        if (!std::binary_search(chars.begin(), chars.end(), c))
            break;
        else
            result++;
    }

    return result;
}

bool pattern::matches(const std::string& s, size_t pos) const
{
    std::string result;

    if (to > 0)
    {
        int size = __pattern_greedyMatch(s, pos, chars);
        if (size < from)
            return false;
        if (size > to)
            size = to;
        pos += size;
    }

    if (children.size() > 0)
    {
        for (size_t child = 0; child < children.size(); child++)
            if (children[child].matches(s, pos))
                return true;
        return false;
    }
    else
        return pos == s.length();
}

std::string pattern::next(random& rnd) const
{
    std::string result;

    if (to == INT_MAX)
        throw "Can't process character '*' for generation";

    if (to > 0)
    {
        int count = rnd.next(to - from + 1) + from;
        for (int i = 0; i < count; i++)
            result += chars[rnd.next(int(chars.size()))];
    }

    if (children.size() > 0)
    {
        int child = rnd.next(int(children.size()));
        result += children[child].next(rnd);
    }

    return result;
}

static void __pattern_scanCounts(const std::string& s, size_t& pos, int& from, int& to)
{
    if (pos >= s.length())
    {
        from = to = 1;
        return;
    }
        
    if (__pattern_isCommandChar(s, pos, '{'))
    {
        std::vector<std::string> parts;
        std::string part;

        pos++;

        while (pos < s.length() && !__pattern_isCommandChar(s, pos, '}'))
        {
            if (__pattern_isCommandChar(s, pos, ','))
                parts.push_back(part), part = "", pos++;
            else
                part += __pattern_getChar(s, pos);
        }

        if (part != "")
            parts.push_back(part);

        if (!__pattern_isCommandChar(s, pos, '}'))
            throw "Illegal pattern";

        pos++;

        if (parts.size() < 1 || parts.size() > 2)
            throw "Illegal pattern";

        std::vector<int> numbers;

        for (size_t i = 0; i < parts.size(); i++)
        {
            if (parts[i].length() == 0)
                throw "Illegal pattern";
            int number;
            if (std::sscanf(parts[i].c_str(), "%d", &number) != 1)
                throw "Illegal pattern";
            numbers.push_back(number);
        }

        if (numbers.size() == 1)
            from = to = numbers[0];
        else
            from = numbers[0], to = numbers[1];

        if (from > to)
            throw "Illegal pattern";
    }
    else
    {
        if (__pattern_isCommandChar(s, pos, '?'))
        {
            from = 0, to = 1, pos++;
            return;
        }

        if (__pattern_isCommandChar(s, pos, '*'))
        {
            from = 0, to = INT_MAX, pos++;
            return;
        }

        if (__pattern_isCommandChar(s, pos, '+'))
        {
            from = 1, to = INT_MAX, pos++;
            return;
        }
        
        from = to = 1;
    }
}

static std::vector<char> __pattern_scanCharSet(const std::string& s, size_t& pos)
{
    if (pos >= s.length())
        throw "Illegal pattern";

    std::vector<char> result;

    if (__pattern_isCommandChar(s, pos, '['))
    {
        pos++;
        bool negative = __pattern_isCommandChar(s, pos, '^');

        char prev = 0;

        while (pos < s.length() && !__pattern_isCommandChar(s, pos, ']'))
        {
            if (__pattern_isCommandChar(s, pos, '-') && prev != 0)
            {
                pos++;

                if (pos + 1 == s.length())
                {
                    result.push_back(prev);
                    prev = '-';
                    continue;
                }
                
                char next = __pattern_getChar(s, pos);

                if (prev > next)
                    throw "Illegal pattern";

                for (char c = prev; c <= next; c++)
                    result.push_back(c);

                prev = 0;
            }
            else
            {
                if (prev != 0)
                    result.push_back(prev);
                prev = __pattern_getChar(s, pos);
            }
        }

        if (prev != 0)
            result.push_back(prev);

        if (!__pattern_isCommandChar(s, pos, ']'))
            throw "Illegal pattern";

        pos++;

        if (negative)
        {
            std::sort(result.begin(), result.end());
            std::vector<char> actuals;
            for (int code = 0; code < 255; code++)
            {
                char c = char(code);
                if (!std::binary_search(result.begin(), result.end(), c))
                    actuals.push_back(c);
            }
            result = actuals;
        }

        std::sort(result.begin(), result.end());
    }
    else
        result.push_back(__pattern_getChar(s, pos));

    return result;
}

pattern::pattern(std::string s): from(0), to(0)
{
    std::string t;
    for (size_t i = 0; i < s.length(); i++)
        if (!__pattern_isCommandChar(s, i, ' '))
            t += s[i];
    s = t;

    int opened = 0;
    int firstClose = -1;
    std::vector<int> seps;

    for (size_t i = 0; i < s.length(); i++)
    {
        if (__pattern_isCommandChar(s, i, '('))
        {
            opened++;
            continue;
        }

        if (__pattern_isCommandChar(s, i, ')'))
        {
            opened--;
            if (opened == 0 && firstClose == -1)
                firstClose = i;
            continue;
        }
        
        if (opened < 0)
            throw "Illegal pattern";

        if (__pattern_isCommandChar(s, i, '|') && opened == 0)
            seps.push_back(i);
    }

    if (opened != 0)
        throw "Illegal pattern";

    if (seps.size() == 0 && firstClose + 1 == (int)s.length() 
            && __pattern_isCommandChar(s, 0, '(') && __pattern_isCommandChar(s, s.length() - 1, ')'))
    {
        children.push_back(pattern(s.substr(1, s.length() - 2)));
    }
    else
    {
        if (seps.size() > 0)
        {
            seps.push_back(s.length());
            int last = 0;

            for (size_t i = 0; i < seps.size(); i++)
            {
                children.push_back(pattern(s.substr(last, seps[i] - last)));
                last = seps[i] + 1;
            }
        }
        else
        {
            size_t pos = 0;
            chars = __pattern_scanCharSet(s, pos);
            __pattern_scanCounts(s, pos, from, to);
            if (pos < s.length())
                children.push_back(pattern(s.substr(pos)));
        }
    }
}
/* End of pattern implementation */

inline bool isEof(char c)
{
    return (c == EOF || c == EOFC);
}

inline bool isEoln(char c)
{
    return (c == LF || c == CR);
}

inline bool isBlanks(char c)
{
    return (c == LF || c == CR || c == SPACE || c == TAB);
}

enum TMode
{
    _input, _output, _answer
};

enum TResult
{
    _ok, _wa, _pe, _fail, _dirt, _partially
};

#define _pc(exitCode) (TResult(_partially + exitCode))

const std::string outcomes[] =
    {"accepted", "wrong-answer", "presentation-error", "fail", "fail", "partially-correct"};

/*
 * Streams to be used for reading data in checkers or validators.
 * Each read*() method moves pointer to the next character after the
 * read value.
 */
struct InStream
{
    /* Do not use it. */
    InStream();

    std::FILE * file;
    std::string name;
    TMode mode;
    bool opened;
    bool stdfile;
    bool strict;

    void init(std::string fileName, TMode mode);
    void init(std::FILE* f, TMode mode);

    /* Moves stream pointer to the first non-white-space character or EOF. */ 
    void skipBlanks();
    
    /* Returns current character in the stream. Doesn't remove it from stream. */
    char curChar();
    /* Moves stream pointer one character forward. */
    void skipChar();
    /* Returns current character and moves pointer one character forward. */
    char nextChar();
    
    /* Returns current character and moves pointer one character forward. */
    char readChar();
    /* As "readChar()" but ensures that the result is equal to given parameter. */
    char readChar(char c);
    /* As "readChar()" but ensures that the result is equal to the space (code=32). */
    char readSpace();
    /* Puts back the character into the stream. */
    void unreadChar(char c);

    /* Reopens stream, you should not use it. */
    void reset();
    /* Checks that current position is EOF. If not it doesn't move stream pointer. */
    bool eof();
    /* Moves pointer to the first non-white-space character and calls "eof()". */
    bool seekEof();

    /* 
     * Checks that current position contains EOLN. 
     * If not it doesn't move stream pointer. 
     * In strict mode expects "#13#10" for windows or "#10" for other platforms.
     */
    bool eoln();
    /* Moves pointer to the first non-space and non-tab character and calls "eoln()". */
    bool seekEoln();

    /* Moves stream pointer to the first character of the next line (if exists). */
    void nextLine();

    /* 
     * Reads new token. Ignores white-spaces into the non-strict mode 
     * (strict mode is used in validators usually). 
     */
    std::string readWord();
    /* The same as "readWord()", it is preffered to use "readToken()". */
    std::string readToken();
    /* The same as "readWord()", but ensures that token matches to given pattern. */
    std::string readWord(const std::string& ptrn);
    /* The same as "readToken()", but ensures that token matches to given pattern. */
    std::string readToken(const std::string& ptrn);

    /* 
     * Reads new long long value. Ignores white-spaces into the non-strict mode 
     * (strict mode is used in validators usually). 
     */
    long long readLong();
    /* 
     * Reads new int. Ignores white-spaces into the non-strict mode 
     * (strict mode is used in validators usually). 
     */
    int readInteger();
    /* 
     * Reads new int. Ignores white-spaces into the non-strict mode 
     * (strict mode is used in validators usually). 
     */
    int readInt();

    /* As "readLong()" but ensures that value in the range [minv,maxv]. */
    long long readLong(long long minv, long long maxv);
    /* As "readInteger()" but ensures that value in the range [minv,maxv]. */
    int readInteger(int minv, int maxv);
    /* As "readInt()" but ensures that value in the range [minv,maxv]. */
    int readInt(int minv, int maxv);

    /* 
     * Reads new double. Ignores white-spaces into the non-strict mode 
     * (strict mode is used in validators usually). 
     */
    double readReal();
    /* 
     * Reads new double. Ignores white-spaces into the non-strict mode 
     * (strict mode is used in validators usually). 
     */
    double readDouble();
    
    /* As "readReal()" but ensures that value in the range [minv,maxv]. */
    double readReal(double minv, double maxv);
    /* As "readDouble()" but ensures that value in the range [minv,maxv]. */
    double readDouble(double minv, double maxv);
    
    /* As readLine(). */
    std::string readString();
    /* 
     * Reads line from the current position to EOLN or EOF. Moves stream pointer to 
     * the first character of the new line (if possible). 
     */
    std::string readLine();

    /* The same as "readLine()", but ensures that line matches to the given pattern. */
    std::string readLine(const std::string& ptrn);

    /* See readLine(const std::string& ptrn). */
    std::string readString(const std::string& ptrn);

    /* Reads EOLN or fails. Use it in validators. Calls "eoln()" method internally. */
    void readEoln();
    /* Reads EOF or fails. Use it in validators. Calls "eof()" method internally. */
    void readEof();

    void quit(TResult result, const char * msg);
    void quits(TResult result, std::string msg);

    void close();

    const static WORD LightGray = 0x07;    
    const static WORD LightRed = 0x0c;    
    const static WORD LightCyan = 0x0b;    
    const static WORD LightGreen = 0x0a;    
    const static WORD LightYellow = 0x0e;    

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
random rnd;

/* implementation
 */

template <typename T>
static std::string vtos(const T& t)
{
    std::string s;
    std::stringstream ss;
    ss << t;
    ss >> s;
    return s;
}

InStream::InStream()
{
    file = NULL;
    name = "";
    mode = _input;
    strict = false;
    stdfile = false;
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
    if (r >= _partially)
        return PC_BASE_EXIT_CODE + (r - _partially);
    return FAIL_EXIT_CODE;
}

void InStream::textColor(WORD color)
{
#ifdef ON_WINDOWS
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

    int pctype = result - _partially;

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
        if (result >= _partially)
        {
            char message[1023];
            std::sprintf(message, "partially correct (%d) ", pctype);
            errorName = std::string(message);
            quitscrS(LightYellow, errorName);
        }
        else
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
            if (result >= _partially)
                fprintf(resultFile, "<result outcome = \"%s\" pctype = \"%d\">", outcomes[(int)_partially].c_str(), pctype);
            else
                fprintf(resultFile, "<result outcome = \"%s\">", outcomes[(int)result].c_str());
            xmlSafeWrite(resultFile, msg);
            fprintf(resultFile, "</result>\n");
        }
        else
             fprintf(resultFile, "%s", msg);
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
    if (opened && stdfile)
        quit(_fail, "Can't reset standard handle");

    if (opened)
        close();

    if (!stdfile)
        if (NULL == (file = std::fopen(name.c_str(), "rb")))
        {
            if (mode == _output)
                quits(_pe, std::string("File not found: \"") + name + "\"");
        }

    opened = true;

#if !defined(unix)
    if (NULL != file)
    {
#ifdef _MSC_VER
        _setmode(_fileno(file), O_BINARY);
#else
        setmode(fileno(file), O_BINARY);
#endif
    }
#endif
}

void InStream::init(std::string fileName, TMode mode)
{
    opened = false;
    name = fileName;
    stdfile = false;
    this->mode = mode;
    reset();
}

void InStream::init(std::FILE* f, TMode mode)
{
    opened = false;
    
    name = "untitled";
    
    if (f == stdin)
        name = "stdin", stdfile = true;
    
    if (f == stdout)
        name = "stdout", stdfile = true;
    
    if (f == stderr)
        name = "stderr", stdfile = true;

    this->file = f;
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

char InStream::readChar()
{
    return nextChar();
}

char InStream::readChar(char c)
{
    char found = readChar();
    if (c != found)
    {
        if (!isEoln(found))
            quit(_pe, ("Unexpected character '" + std::string(1, found) + "', but '" + std::string(1, c) + "' expected").c_str());
        else
            quit(_pe, ("Unexpected character " + ("#" + vtos(int(found))) + ", but '" + std::string(1, c) + "' expected").c_str());
    }
    return found;
}

char InStream::readSpace()
{
    return readChar(' ');
}

void InStream::unreadChar(char c)
{
    ungetc(c, file);
}

void InStream::skipChar()
{
    getc(file);
}

void InStream::skipBlanks()
{
    char cur;
    while (isBlanks(cur = readChar()));
    unreadChar(cur);
}

std::string InStream::readWord()
{
    if (!strict)
        skipBlanks();

    char cur = readChar();

    if (isEof(cur))
        quit(_pe, "Unexpected end of file - token expected");

    if (isBlanks(cur))
        quit(_pe, "Unexpected white-space - token expected");

    std::string result = "";

    while (!(isBlanks(cur) || cur == EOF))
    {
        result += cur;
        cur = nextChar();
    }

    unreadChar(cur);

    if (result.length() == 0)
        quit(_pe, "Unexpected end of file or white-space - token expected");

    return result;
}

std::string InStream::readToken()
{
    return readWord();
}

static std::string __testlib_part(const std::string& s)
{
    if (s.length() <= 64)
        return s;
    else
        return s.substr(0, 30) + "..." + s.substr(s.length() - 31, 31);
}

std::string InStream::readWord(const std::string& ptrn)
{
    pattern p(ptrn);
    std::string result = readWord();
    if (!p.matches(result))
        quit(_pe, ("Token \"" + __testlib_part(result) + "\" doesn't correspond to pattern \"" + ptrn + "\"").c_str());
    return result;
}

std::string InStream::readToken(const std::string& ptrn)
{
    return readWord(ptrn);
}

static bool equals(long long integer, const char* s)
{
    if (integer == LLONG_MIN)
        return strcmp(s, "-9223372036854775808") == 0;

    if (integer == 0LL)
        return strcmp(s, "0") == 0;

    size_t length = strlen(s);

    if (length == 0)
        return false;

    if (integer < 0 && s[0] != '-')
        return false;

    if (integer < 0)
        s++, length--, integer = -integer;

    if (length == 0)
        return false;

    while (integer > 0)
    {
        int digit = integer % 10;

        if (s[length - 1] != '0' + digit)
            return false;

        length--;
        integer /= 10;
    }

    return length == 0;
}

static double stringToDouble(InStream& in, const char* buffer)
{
    double retval;

    size_t length = strlen(buffer);

    for (size_t i = 0; i < length; i++)
        if (isBlanks(buffer[i]))
            in.quit(_pe, ("Expected double, but \"" + (std::string)buffer + "\" found").c_str());

    if (std::sscanf(buffer, "%lf", &retval) == 1)
        return retval;
    else
        in.quit(_pe, ("Expected double, but \"" + (std::string)buffer + "\" found").c_str());

    throw "Unexpected exception";
}

static long long stringToLongLong(InStream& in, const char* buffer)
{
    if (strcmp(buffer, "-9223372036854775808") == 0)
        return LLONG_MIN;

    bool minus = false;
    size_t length = strlen(buffer);
    
    if (length > 1 && buffer[0] == '-')
        minus = true;

    if (length > 20)
        in.quit(_pe, ("Expected integer, but \"" + (std::string)buffer + "\" found").c_str());

    long long retval = 0LL;

    int zeroes = 0;
    int processingZeroes = true;
    
    for (size_t i = (minus ? 1 : 0); i < length; i++)
    {
        if (buffer[i] == '0' && processingZeroes)
            zeroes++;
        else
            processingZeroes = false;

        if (buffer[i] < '0' || buffer[i] > '9')
            in.quit(_pe, ("Expected integer, but \"" + (std::string)buffer + "\" found").c_str());
        retval = retval * 10 + (buffer[i] - '0');
    }

    if (retval < 0)
        in.quit(_pe, ("Expected integer, but \"" + (std::string)buffer + "\" found").c_str());
    
    if ((zeroes > 0 && (retval != 0 || minus)) || zeroes > 1)
        in.quit(_pe, ("Expected integer, but \"" + (std::string)buffer + "\" found").c_str());

    retval = (minus ? -retval : +retval);

    if (length < 19)
        return retval;

    if (equals(retval, buffer))
        return retval;
    else
        in.quit(_pe, ("Expected int64, but \"" + (std::string)buffer + "\" found").c_str());

    throw "Unexpected exception";
}

int InStream::readInteger()
{
    if (!strict && seekEof())
        quit(_pe, "Unexpected end of file - int32 expected");

    std::string token = readWord();
    long long value = stringToLongLong(*this, token.c_str());
    if (value < INT_MIN || value > INT_MAX)
        quit(_pe, ("Expected int32, but \"" + token + "\" found").c_str());
    return int(value);
}

long long InStream::readLong()
{
    if (!strict && seekEof())
        quit(_pe, "Unexpected end of file - int64 expected");

    std::string token = readWord();
    return stringToLongLong(*this, token.c_str());
}

long long InStream::readLong(long long minv, long long maxv)
{
    long long result = readLong();

    if (result < minv || result > maxv)
        quit(_pe, ("Integer " + vtos(result) + " violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());

    return result;
}

int InStream::readInt()
{
    return readInteger();
}

int InStream::readInt(int minv, int maxv)
{
    int result = readInt();

    if (result < minv || result > maxv)
        quit(_pe, ("Integer " + vtos(result) + " violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());

    return result;
}

int InStream::readInteger(int minv, int maxv)
{
    return readInt(minv, maxv);
}

double InStream::readReal()
{
    if (!strict && seekEof())
        quit(_pe, "Unexpected end of file - double expected");

    return stringToDouble(*this, readWord().c_str());
}

double InStream::readDouble()
{
    return readReal();
}

double InStream::readReal(double minv, double maxv)
{
    double result = readReal();

    if (result < minv || result > maxv)
        quit(_pe, ("Double " + vtos(result) + " violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());

    return result;
}

double InStream::readDouble(double minv, double maxv)
{
    return readReal(minv, maxv);
}

bool InStream::eof()
{
    if (!strict && NULL == file)
        return true;

    if (feof(file) != 0)
        return true;
    else
    {
        int cur = getc(file);

        if (isEof(char(cur)))
            return true;
        else
        {
            ungetc(cur, file);
            return false;
        }
    }
}

bool InStream::seekEof()
{
    if (NULL == file)
        return true;
    skipBlanks();
    return eof();
}

bool InStream::eoln()
{
    if (!strict && NULL == file)
        return true;

    char c = nextChar();

    if (!strict)
    {
        if (isEof(c))
            return true;

        if (c == CR)
        {
            c = nextChar();

            if (c != LF)
            {
                unreadChar(CR);
                unreadChar(c);
                return false;
            }
            else
                return true;
        }
        
        if (c == LF)
            return true;

        unreadChar(c);
        return false;
    }
    else
    {
        bool returnCr = false;

#ifdef ON_WINDOWS
        if (c != CR)
        {
            unreadChar(c);
            return false;
        }
        else
        {
            if (!returnCr)
                returnCr = true;
            c = nextChar();
        }
#endif        
        if (c != LF)
        {
            if (returnCr)
                unreadChar(CR);
            unreadChar(LF);
            return false;
        }

        return true;
    }
}

void InStream::readEoln()
{
    if (!eoln())
        quit(_pe, "Expected EOLN");
}

void InStream::readEof()
{
    if (!eof())
        quit(_pe, "Expected EOF");
}

bool InStream::seekEoln()
{
    if (NULL == file)
        return true;
    
    char cur;
    do
    {
        cur = nextChar();
    } 
    while (cur == SPACE || cur == TAB);
    ungetc(cur, file);

    return eoln();
}

void InStream::nextLine()
{
    if (NULL == file)
        return;
    char cur;
    while (!isEoln(cur = readChar()));
    if (cur == CR)
    {
        cur = readChar();
        if (cur != LF)
            unreadChar(cur);
    }
    else
    {
        if (cur != LF)
            unreadChar(cur);
    }
}

std::string InStream::readString()
{
    if (NULL == file)
        quit(_pe, "Expected line");

    std::string retval = "";
    char cur;

    for (;;)
    {
        cur = readChar();

        if (isEoln(cur))
            break;

        if (isEof(cur))
            break;

        retval += cur;
    }

    unreadChar(cur);

    if (strict)
        readEoln();
    else
        eoln();

    return retval;
}

std::string InStream::readString(const std::string& ptrn)
{
    pattern p(ptrn);
    std::string result = readString();
    if (!p.matches(result))
        quit(_pe, ("Line \"" + __testlib_part(result) + "\" doesn't correspond to pattern \"" + ptrn + "\"").c_str());
    return result;
}

std::string InStream::readLine()
{
    return readString();
}

std::string InStream::readLine(const std::string& ptrn)
{
    return readString(ptrn);    
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
    char * buffer = new char [MAX_FORMAT_BUFFER_SIZE];
    
    va_list ap;
    va_start(ap, format);
    std::vsprintf(buffer, format, ap);
    va_end(ap);

    std::string output(buffer);
    delete[] buffer;

    quit(result, output);
}

void registerGen(int argc, char* argv[])
{
    rnd.setSeed(argc, argv);
}

void registerValidation()
{
    inf.init(stdin, _input);
    inf.strict = true;
}

void registerTestlibCmd(int argc, char * argv[])
{
    if (argc > 1 && !strcmp("--help", argv[1]))
    {
        InStream::textColor(InStream::LightCyan);
        std::printf("TESTLIB %s ", VERSION);
        std::printf("by Mike Mirzayanov, copyright(c) 2005-2009\n");
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

    // testlib assumes: sizeof(int) = 4.
    __TESTLIB_STATIC_ASSERT(sizeof(int) == 4);

    // testlib assumes: sizeof(long long) = 8.
    __TESTLIB_STATIC_ASSERT(sizeof(long long) == 8);

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
                if(__testlib_abs(result - expected) < MAX_DOUBLE_ERROR)
                {
                    return true;
                }
                else
                {
                    double minv = __testlib_min(expected * (1.0 - MAX_DOUBLE_ERROR),
                                 expected * (1.0 + MAX_DOUBLE_ERROR));
                    double maxv = __testlib_max(expected * (1.0 - MAX_DOUBLE_ERROR),
                                  expected * (1.0 + MAX_DOUBLE_ERROR));
                    return result > minv && result < maxv;
                }
}

double doubleDelta(double expected, double result)
{
    double absolute = __testlib_abs(result - expected);
    
    if (__testlib_abs(expected) > 1E-9)
    {
        double relative = __testlib_abs(absolute / expected);
        return __testlib_min(absolute, relative);
    }
    else
        return absolute;
}

static void __testlib_ensure(bool cond, const std::string msg)
{
    if (!cond)
        quitf(_fail, msg.c_str());
}

#define ensure(cond) __testlib_ensure(cond, std::string("Condition failed: \"") + #cond + "\"")

void ensuref(bool cond, const char* format, ...)
{
    char * buffer = new char [MAX_FORMAT_BUFFER_SIZE];
    
    va_list ap;
    va_start(ap, format);
    std::vsprintf(buffer, format, ap);
    va_end(ap);

    std::string message(buffer);
    delete[] buffer;

    __testlib_ensure(cond, message);
}

void setName(const char* format, ...)
{
    char * buffer = new char [MAX_FORMAT_BUFFER_SIZE];
    
    va_list ap;
    va_start(ap, format);
    std::vsprintf(buffer, format, ap);
    va_end(ap);

    std::string name(buffer);
    delete[] buffer;

    checkerName = name;
}

/* 
 * Do not use random_shuffle, because it will produce different result
 * for different C++ compilers.
 *
 * This implementation uses testlib random to produce random numbers, so
 * it is stable.
 */ 
template<typename _RandomAccessIter>
void shuffle(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        iter_swap(__i, __first + rnd.next((__i - __first) + 1));
}


template<typename _RandomAccessIter>
void random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last)
{
    quitf(_fail, "Don't use random_shuffle(), use shuffle()");
}

int rand()
{
    quitf(_fail, "Don't use rand(), use rnd.next()");
    return 0;
}

void srand(unsigned int seed)
{
    quitf(_fail, "Don't use srand(), you should use" 
        "'registerGen(argc, argv);' to initialize generator.", seed);
}

#endif
