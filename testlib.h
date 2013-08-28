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
 * Please read the documentation for class "random_t" and use "rnd" instance in
 * generators. Probably, these sample calls will be usefull for you:
 *              rnd.next(); rnd.next(100); rnd.next(1, 2); 
 *              rnd.next(3.14); rnd.next("[a-z]{1,100}").
 *
 * Also read about wnext() to generate off-center random distribution.
 *
 * See http://code.google.com/p/testlib/ to get latest version or bug tracker.
 */

#ifndef _TESTLIB_H_
#define _TESTLIB_H_

/*
 * Copyright (c) 2005-2013
 */

#define VERSION "0.9.2-SNAPSHOT"

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
 *     gen.exe [parameter-1] [parameter-2] [... paramerter-n]
 *   You can write generated test(s) into standard output or into the file(s).
 *
 *   Interactor, using testlib running format:                                          
 *     interactor.exe <Input_File> <Output_File> [<Answer_File> [<Result_File> [-appes]]],
 *   Reads test from inf (mapped to args[1]), writes result to tout (mapped to argv[2],
 *   can be judged by checker later), reads program output from ouf (mapped to stdin),
 *   writes output to program via stdout (use cout, printf, etc).
 */

const char* latestFeatures[] = {
                          "Fixed incorrect interval parsing in patterns",
                          "Use registerGen(argc, argv, 1) to develop new generator, use registerGen(argc, argv, 0) to compile old generators (originally created for testlib under 0.8.7)",
                          "Introduced disableFinalizeGuard() to switch off finalization checkings",
                          "Use join() functions to format a range of items as a single string (separated by spaces or other separators)",
                          "Use -DENABLE_UNEXPECTED_EOF to enable special exit code (by default, 8) in case of unexpected eof. It is good idea to use it in interactors",
                          "Use -DUSE_RND_AS_BEFORE_087 to compile in compatibility mode with random behavior of versions before 0.8.7",
                          "Fixed bug with nan in stringToDouble", 
                          "Fixed issue around overloads for size_t on x64",  
                          "Added attribute 'points' to the XML output in case of result=_points",  
                          "Exit codes can be customized via macros, e.g. -DPE_EXIT_CODE=14",  
                          "Introduced InStream function readWordTo/readTokenTo/readStringTo/readLineTo for faster reading",  
                          "Introduced global functions: format(), englishEnding(), upperCase(), lowerCase(), compress()",  
                          "Manual buffer in InStreams, some IO speed improvements",  
                          "Introduced quitif(bool, const char* pattern, ...) which delegates to quitf() in case of first argument is true",  
                          "Introduced guard against missed quitf() in checker or readEof() in validators",  
                          "Supported readStrictReal/readStrictDouble - to use in validators to check strictly float numbers",  
                          "Supported registerInteraction(argc, argv)",  
                          "Print checker message to the stderr instead of stdout",  
                          "Supported TResult _points to output calculated score, use quitp(...) functions",  
                          "Fixed to be compilable on Mac",  
                          "PC_BASE_EXIT_CODE=50 in case of defined TESTSYS",
                          "Fixed issues 19-21, added __attribute__ format printf",  
                          "Some bug fixes",  
                          "ouf.readInt(1, 100) and similar calls return WA",  
                          "Modified random_t to avoid integer overflow",  
                          "Truncated checker output [patch by Stepan Gatilov]",  
                          "Renamed class random -> class random_t",  
                          "Supported name parameter for read-and-validation methods, like readInt(1, 2, \"n\")",  
                          "Fixed bug in readDouble()",  
                          "Improved ensuref(), fixed nextLine to work in case of EOF, added startTest()",  
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
#include <cmath>
#include <sstream>
#include <fstream>
#include <cstring>
#include <limits>
#include <stdarg.h>

#include <fcntl.h>

#if ( _WIN32 || __WIN32__ || _WIN64 || __WIN64__ )
#   if !defined(_MSC_VER) || _MSC_VER>1400
#       include <windows.h>
#   else
#       define WORD unsigned short
#   endif
#   include <io.h>
#   define ON_WINDOWS
#else
#   define WORD unsigned short
#endif

#ifndef LLONG_MIN
#define LLONG_MIN   (-9223372036854775807LL - 1)
#endif

#define LF ((char)10)
#define CR ((char)13)
#define TAB ((char)9)
#define SPACE ((char)' ')
#define EOFC ((char)26)

#ifndef OK_EXIT_CODE
#   define OK_EXIT_CODE 0
#endif

#ifndef WA_EXIT_CODE
#   ifdef EJUDGE
#       define WA_EXIT_CODE 5
#   else
#       define WA_EXIT_CODE 1
#   endif
#endif

#ifndef PE_EXIT_CODE
#   ifdef EJUDGE
#       define PE_EXIT_CODE 4
#   else
#       define PE_EXIT_CODE 2
#   endif
#endif

#ifndef FAIL_EXIT_CODE
#   ifdef EJUDGE
#       define FAIL_EXIT_CODE 6
#   else
#       define FAIL_EXIT_CODE 3
#   endif
#endif

#ifndef DIRT_EXIT_CODE
#   ifdef EJUDGE
#       define DIRT_EXIT_CODE 6
#   else
#       define DIRT_EXIT_CODE 4
#   endif
#endif

#ifndef POINTS_EXIT_CODE
#   define POINTS_EXIT_CODE 0
#endif

#ifndef UNEXPECTED_EOF_EXIT_CODE
#   define UNEXPECTED_EOF_EXIT_CODE 8
#endif

#ifndef PC_BASE_EXIT_CODE
#   ifdef TESTSYS
#       define PC_BASE_EXIT_CODE 50
#   else
#       define PC_BASE_EXIT_CODE 0
#   endif
#endif

#ifdef __GNUC__
#    define __TESTLIB_STATIC_ASSERT(condition) typedef void* __testlib_static_assert_type[(condition) ? 1 : -1] __attribute__((unused))
#else
#    define __TESTLIB_STATIC_ASSERT(condition) typedef void* __testlib_static_assert_type[(condition) ? 1 : -1]
#endif

#ifdef ON_WINDOWS
#define I64 "%I64d"
#define U64 "%I64u"
#else
#define I64 "%lld"
#define U64 "%llu"
#endif

#ifdef _MSC_VER
#   define NORETURN __declspec(noreturn)
#elif defined __GNUC__
#   define NORETURN __attribute__ ((noreturn))
#else
#   define NORETURN
#endif
                   
const size_t INLINE_CHAR_BUFFER_SIZE = 131072;

static char __testlib_format_buffer[16777216];
static int __testlib_format_buffer_usage_count = 0;

#define FMT_TO_RESULT(fmt, cstr, result)  std::string result;                              \
            if (__testlib_format_buffer_usage_count != 0)                                  \
                __testlib_fail("FMT_TO_RESULT::__testlib_format_buffer_usage_count != 0"); \
            __testlib_format_buffer_usage_count++;                                         \
            va_list ap;                                                                    \
            va_start(ap, fmt);                                                             \
            std::vsprintf(__testlib_format_buffer, cstr, ap);                              \
            va_end(ap);                                                                    \
            result = std::string(__testlib_format_buffer);                                 \
            __testlib_format_buffer_usage_count--;                                         \

const long long __TESTLIB_LONGLONG_MAX = 9223372036854775807LL;

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

static bool __testlib_prelimIsNaN(double r)
{
    volatile double ra = r;
#ifndef __BORLANDC__
    return ((ra != ra) == true) && ((ra == ra) == false) && ((1.0 > ra) == false) && ((1.0 < ra) == false);
#else
    return std::_isnan(ra);
#endif
}

static bool __testlib_isNaN(double r)
{
    __TESTLIB_STATIC_ASSERT(sizeof(double) == sizeof(long long));
    volatile double ra = r;
    long long llr1, llr2;
    std::memcpy((void*)&llr1, (void*)&ra, sizeof(double)); 
    ra = -ra;
    std::memcpy((void*)&llr2, (void*)&ra, sizeof(double)); 
    long long llnan = 0xFFF8000000000000;
    return __testlib_prelimIsNaN(r) || llnan == llr1 || llnan == llr2;
}

static double __testlib_nan()
{
    __TESTLIB_STATIC_ASSERT(sizeof(double) == sizeof(long long));
#ifndef NAN
    long long llnan = 0xFFF8000000000000;
    double nan;
    std::memcpy(&nan, &llnan, sizeof(double));
    return nan;
#else
    return NAN;
#endif
}

static bool __testlib_isInfinite(double r)
{
    volatile double ra = r;
    return (ra > 1E100 || ra < -1E100);
}

NORETURN static void __testlib_fail(const std::string& message);

static void __testlib_set_binary(std::FILE* file)
{
#ifdef O_BINARY
    if (NULL != file)
    {
#ifndef __BORLANDC__
        _setmode(_fileno(file), O_BINARY);
#else
        setmode(fileno(file), O_BINARY);
#endif
    }
#endif
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
 * "p.matches(std::string s)" or "p.next(random_t& rd)" to check matching or generate
 * new string by pattern.
 * 
 * Simpler way to read token and check it for pattern matching is "inf.readToken("[a-z]+")".
 */
class random_t;

class pattern
{
public:
    /* Create pattern instance by string. */
    pattern(std::string s);
    /* Generate new string by pattern and given random_t. */
    std::string next(random_t& rnd) const;
    /* Checks if given string match the pattern. */
    bool matches(const std::string& s) const;
    /* Returns source string of the pattern. */
    std::string src() const;
private:
    bool matches(const std::string& s, size_t pos) const;

    std::string s;
    std::vector<pattern> children;
    std::vector<char> chars;
    int from;
    int to;
};

/* 
 * Use random_t instances to generate random values. It is preffered
 * way to use randoms instead of rand() function or self-written 
 * randoms.
 *
 * Testlib defines global variable "rnd" of random_t class.
 * Use registerGen(argc, argv, 1) to setup random_t seed be command
 * line (to use latest random generator version).
 *
 * Random generates uniformly distributed values if another strategy is
 * not specified explicitly.
 */
class random_t
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
                __testlib_fail("random_t::nextBits(int bits): n must be less than 64");

            int lowerBitCount = (random_t::version == 0 ? 31 : 32);
            return ((nextBits(31) << 32) ^ nextBits(lowerBitCount));
        }
    }

public:
    static int version;

    /* New random_t with fixed seed. */
    random_t()
        : seed(3905348978240129619LL)
    {
    }

    /* Sets seed by command line. */
    void setSeed(int argc, char* argv[])
    {
        random_t p;

        seed = 3905348978240129619LL;
        for (int i = 1; i < argc; i++)
        {
            std::size_t le = std::strlen(argv[i]);
            for (std::size_t j = 0; j < le; j++)
                seed = seed * multiplier + (unsigned int)(argv[i][j]) + addend;
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

#ifndef __BORLANDC__
    /* Random string value by given pattern (see pattern documentation). */
    std::string next(const std::string& ptrn)
    {
        pattern p(ptrn);
        return p.next(*this);
    }
#else
    /* Random string value by given pattern (see pattern documentation). */
    std::string next(std::string ptrn)
    {
        pattern p(ptrn);
        return p.next(*this);
    }
#endif

    /* Random value in range [0, n-1]. */
    int next(int n)
    {
        if (n <= 0)
            __testlib_fail("random_t::next(int n): n must be positive");

        if ((n & -n) == n)  // n is a power of 2
            return (int)((n * (long long)nextBits(31)) >> 31);

        const long long limit = INT_MAX / n * n;
        
        long long bits;
        do {
            bits = nextBits(31);
        } while (bits >= limit);

        return int(bits % n);
    }

    /* Random value in range [0, n-1]. */
    unsigned int next(unsigned int n)
    {
        if (n >= INT_MAX)
            __testlib_fail("random_t::next(unsigned int n): n must be less INT_MAX");
        return (unsigned int)next(int(n));
    }

    /* Random value in range [0, n-1]. */
    long long next(long long n) 
    {
        if (n <= 0)
            __testlib_fail("random_t::next(long long n): n must be positive");

        const long long limit = __TESTLIB_LONGLONG_MAX / n * n;
        
        long long bits;
        do {
            bits = nextBits(63);
        } while (bits >= limit);

        return bits % n;
    }

    /* Random value in range [0, n-1]. */
    unsigned long long next(unsigned long long n)
    {
        if (n >= (unsigned long long)(__TESTLIB_LONGLONG_MAX))
            __testlib_fail("random_t::next(unsigned long long n): n must be less LONGLONG_MAX");
        return (unsigned long long)next((long long)(n));
    }

    /* Random value in range [0, n-1]. */
    long next(long n)
    {
        return (long)next((long long)(n));
    }

    /* Random value in range [0, n-1]. */
    unsigned long next(unsigned long n)
    {
        if (n >= (unsigned long)(LONG_MAX))
            __testlib_fail("random_t::next(unsigned long n): n must be less LONG_MAX");
        return (unsigned long)next((unsigned long long)(n));
    }

    /* Returns random value in range [from,to]. */
    int next(int from, int to)
    {
        return int(next((long long)to - from + 1) + from);
    }

    /* Returns random value in range [from,to]. */
    unsigned int next(unsigned int from, unsigned int to)
    {
        return (unsigned int)(next((long long)to - from + 1) + from);
    }

    /* Returns random value in range [from,to]. */
    long long next(long long from, long long to)
    {
        return next(to - from + 1) + from;
    }

    /* Returns random value in range [from,to]. */
    unsigned long long next(unsigned long long from, unsigned long long to)
    {
        if (from > to)
            __testlib_fail("random_t::next(unsigned long long from, unsigned long long to): from can't not exceed to");
        return next(to - from + 1) + from;
    }

    /* Returns random value in range [from,to]. */
    long next(long from, long to)
    {
        return next(to - from + 1) + from;
    }

    /* Returns random value in range [from,to]. */
    unsigned long next(unsigned long from, unsigned long to)
    {
        if (from > to)
            __testlib_fail("random_t::next(unsigned long from, unsigned long to): from can't not exceed to");
        return next(to - from + 1) + from;
    }

    /* Random double value in range [0, 1). */
    double next() 
    {
        return (double)(((long long)(nextBits(26)) << 27) + nextBits(27)) / (double)(1LL << 53);
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

    /* Returns random element from container. */
    template <typename Container>
    typename Container::value_type any(const Container& c)
    {
        size_t size = c.size();
        if (size <= 0)
            __testlib_fail("random_t::any(const Container& c): c.size() must be positive");
        return *(c.begin() + next(size));
    }

    /* Returns random element from iterator range. */
    template <typename Iter>
    typename Iter::value_type any(const Iter& begin, const Iter& end)
    {
        int size = int(end - begin);
        if (size <= 0)
            __testlib_fail("random_t::any(const Iter& begin, const Iter& end): range must have positive length");
        return *(begin + next(size));
    }

    /* Random string value by given pattern (see pattern documentation). */
#ifdef __GNUC__
    __attribute__ ((format (printf, 2, 3)))
#endif
    std::string next(const char* format, ...)
    {
        FMT_TO_RESULT(format, format, ptrn);
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
            __testlib_fail("random_t::wnext(int n, int type): n must be positive");
        
        if (abs(type) < random_t::lim)
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
    long long wnext(long long n, int type)
    {
        if (n <= 0)
            __testlib_fail("random_t::wnext(long long n, int type): n must be positive");
        
        if (abs(type) < random_t::lim)
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

            return __testlib_min(__testlib_max((long long)(double(n) * p), 0LL), n - 1LL);
        }
    }
    
    /* See wnext(int, int). It uses the same algorithms. */
    double wnext(int type)
    {
        if (abs(type) < random_t::lim)
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
            __testlib_fail("random_t::wnext(double n, int type): n must be positive");

        if (abs(type) < random_t::lim)
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

    /* See wnext(int, int). It uses the same algorithms. */
    unsigned int wnext(unsigned int n, int type)
    {
        if (n >= INT_MAX)
            __testlib_fail("random_t::wnext(unsigned int n, int type): n must be less INT_MAX");
        return (unsigned int)wnext(int(n), type);
    }
    
    /* See wnext(int, int). It uses the same algorithms. */
    unsigned long long wnext(unsigned long long n, int type)
    {
        if (n >= (unsigned long long)(__TESTLIB_LONGLONG_MAX))
            __testlib_fail("random_t::wnext(unsigned long long n, int type): n must be less LONGLONG_MAX");

        return (unsigned long long)wnext((long long)(n), type);
    }

    /* See wnext(int, int). It uses the same algorithms. */
    long wnext(long n, int type)
    {
        return (long)wnext((long long)(n), type);
    }
    
    /* See wnext(int, int). It uses the same algorithms. */
    unsigned long wnext(unsigned long n, int type)
    {
        if (n >= (unsigned long)(LONG_MAX))
            __testlib_fail("random_t::wnext(unsigned long n, int type): n must be less LONG_MAX");

        return (unsigned long)wnext((unsigned long long)(n), type);
    }

    /* Returns weighted random value in range [from, to]. */
    int wnext(int from, int to, int type)
    {
        return wnext(to - from + 1, type) + from;
    }
    
    /* Returns weighted random value in range [from, to]. */
    int wnext(unsigned int from, unsigned int to, int type)
    {
        return int(wnext(to - from + 1, type) + from);
    }
    
    /* Returns weighted random value in range [from, to]. */
    long long wnext(long long from, long long to, int type)
    {
        return wnext(to - from + 1, type) + from;
    }
    
    /* Returns weighted random value in range [from, to]. */
    unsigned long long wnext(unsigned long long from, unsigned long long to, int type)
    {
        if (from > to)
            __testlib_fail("random_t::wnext(unsigned long long from, unsigned long long to, int type): from can't not exceed to");
        return wnext(to - from + 1, type) + from;
    }
    
    /* Returns weighted random value in range [from, to]. */
    long wnext(long from, long to, int type)
    {
        return wnext(to - from + 1, type) + from;
    }
    
    /* Returns weighted random value in range [from, to]. */
    unsigned long wnext(unsigned long from, unsigned long to, int type)
    {
        if (from > to)
            __testlib_fail("random_t::wnext(unsigned long from, unsigned long to, int type): from can't not exceed to");
        return wnext(to - from + 1, type) + from;
    }
    
    /* Returns weighted random double value in range [from, to). */
    double wnext(double from, double to, int type)
    {
        return wnext(to - from, type) + from;
    }

    /* Returns weighted random element from container. */
    template <typename Container>
    typename Container::value_type wany(const Container& c, int type)
    {
        size_t size = c.size();
        if (size <= 0)
            __testlib_fail("random_t::wany(const Container& c, int type): c.size() must be positive");
        return *(c.begin() + wnext(size, type));
    }

    /* Returns weighted random element from iterator range. */
    template <typename Iter>
    typename Iter::value_type wany(const Iter& begin, const Iter& end, int type)
    {
        int size = int(end - begin);
        if (size <= 0)
            __testlib_fail("random_t::any(const Iter& begin, const Iter& end, int type): range must have positive length");
        return *(begin + wnext(size, type));
    }
};

const int random_t::lim = 25;
const long long random_t::multiplier = 0x5DEECE66DLL;
const long long random_t::addend = 0xBLL;
const long long random_t::mask = (1LL << 48) - 1;
int random_t::version = -1;

/* Pattern implementation */
bool pattern::matches(const std::string& s) const
{
    return matches(s, 0);
}

static bool __pattern_isSlash(const std::string& s, size_t pos)
{
    return s[pos] == '\\';
}

#ifdef __GNUC__
__attribute__((pure))
#endif
static bool __pattern_isCommandChar(const std::string& s, size_t pos, char value)
{
    if (pos >= s.length())
        return false;

    int slashes = 0;

    int before = int(pos) - 1;
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

#ifdef __GNUC__
__attribute__((pure))
#endif
static int __pattern_greedyMatch(const std::string& s, size_t pos, const std::vector<char> chars)
{
    int result = 0;

    while (pos < s.length())
    {
        char c = s[pos++];
        if (!std::binary_search(chars.begin(), chars.end(), c))
            break;
        else
            result++;
    }

    return result;
}

std::string pattern::src() const
{
    return s;
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

std::string pattern::next(random_t& rnd) const
{
    std::string result;
    result.reserve(20);

    if (to == INT_MAX)
        __testlib_fail("pattern::next(random_t& rnd): can't process character '*' for generation");

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
            __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");

        pos++;

        if (parts.size() < 1 || parts.size() > 2)
            __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");

        std::vector<int> numbers;

        for (size_t i = 0; i < parts.size(); i++)
        {
            if (parts[i].length() == 0)
                __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");
            int number;
            if (std::sscanf(parts[i].c_str(), "%d", &number) != 1)
                __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");
            numbers.push_back(number);
        }

        if (numbers.size() == 1)
            from = to = numbers[0];
        else
            from = numbers[0], to = numbers[1];

        if (from > to)
            __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");
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
        __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");

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

                if (pos + 1 == s.length() || __pattern_isCommandChar(s, pos, ']'))
                {
                    result.push_back(prev);
                    prev = '-';
                    continue;
                }

                char next = __pattern_getChar(s, pos);
                if (prev > next)
                    __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");

                for (char c = prev; c != next; c++)
                    result.push_back(c);
                result.push_back(next);

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
            __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");

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

pattern::pattern(std::string s): s(s), from(0), to(0)
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
                firstClose = int(i);
            continue;
        }
        
        if (opened < 0)
            __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");

        if (__pattern_isCommandChar(s, i, '|') && opened == 0)
            seps.push_back(int(i));
    }

    if (opened != 0)
        __testlib_fail("pattern: Illegal pattern (or part) \"" + s + "\"");

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

template <typename C>
inline bool isEof(C c)
{
    return (c == EOF || c == EOFC);
}

template <typename C>
inline bool isEoln(C c)
{
    return (c == LF || c == CR);
}

template<typename C>
inline bool isBlanks(C c)
{
    return (c == LF || c == CR || c == SPACE || c == TAB);
}

enum TMode
{
    _input, _output, _answer
};

/* Outcomes 6-15 are reserved for future use. */
enum TResult
{
    _ok = 0,
    _wa = 1,
    _pe = 2,
    _fail = 3,
    _dirt = 4,
    _points = 5,
    _unexpected_eof = 8,
    _partially = 16
};

enum TTestlibMode
{
    _unknown, _checker, _validator, _generator, _interactor
};

#define _pc(exitCode) (TResult(_partially + (exitCode)))

/* Outcomes 6-15 are reserved for future use. */
const std::string outcomes[] = {
    "accepted",
    "wrong-answer",
    "presentation-error",
    "fail",
    "fail",
#ifndef PCMS2
    "points",
#else
    "relative-scoring",
#endif
    "reserved",
    "reserved",
    "unexpected-eof",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "partially-correct"
};

class InputStreamReader
{
public:
    virtual int curChar() = 0;    
    virtual int nextChar() = 0;    
    virtual void skipChar() = 0;
    virtual void unreadChar(int c) = 0;
    virtual std::string getName() = 0;
    virtual bool eof() = 0;
    virtual ~InputStreamReader() = 0;
};

InputStreamReader::~InputStreamReader()
{
    // No operations.
}

class FileInputStreamReader: public InputStreamReader
{
private:
    std::FILE* file;
    std::string name;

public:
    FileInputStreamReader(std::FILE* file, const std::string& name): file(file), name(name)
    {
        // No operations.
    }

    int curChar()
    {
        if (feof(file))
            return EOF;
        else
        {
            int c = getc(file);
            ungetc(c, file);
            return c;
        }
    }

    int nextChar()
    {
        if (feof(file))
            return EOF;
        else
            return getc(file);
    }

    void skipChar()
    {
        getc(file);
    }

    void unreadChar(int c)
    {   
        ungetc(c, file);
    }

    std::string getName()
    {
        return name;
    }

    bool eof()
    {
        if (feof(file))
            return true;
        else
        {
            int c = getc(file);
            bool result = (c == EOF);
            ungetc(c, file);
            return result;
        }
    }
};

class BufferedFileInputStreamReader: public InputStreamReader
{
private:
    static const size_t BUFFER_SIZE;
    static const size_t MAX_UNREAD_COUNT; 
    
    std::FILE* file;
    char* buffer;
    bool* isEof;
    int bufferPos;
    size_t bufferSize;

    std::string name;

    bool refill()
    {
        if (NULL == file)
            __testlib_fail("BufferedFileInputStreamReader: file == NULL (" + getName() + ")");

        if (bufferPos >= int(bufferSize))
        {
            size_t readSize = fread(
                buffer + MAX_UNREAD_COUNT,
                1,
                BUFFER_SIZE - MAX_UNREAD_COUNT,
                file
            );

            if (readSize < BUFFER_SIZE - MAX_UNREAD_COUNT
                    && ferror(file))
                __testlib_fail("BufferedFileInputStreamReader: unable to read (" + getName() + ")");

            bufferSize = MAX_UNREAD_COUNT + readSize;
            bufferPos = int(MAX_UNREAD_COUNT);
            std::memset(isEof + MAX_UNREAD_COUNT, 0, sizeof(isEof[0]) * readSize);

            return readSize > 0;
        }
        else
            return true;
    }

public:
    BufferedFileInputStreamReader(std::FILE* file, const std::string& name): file(file), name(name)
    {
        buffer = new char[BUFFER_SIZE];
        isEof = new bool[BUFFER_SIZE];
        bufferSize = MAX_UNREAD_COUNT;
        bufferPos = int(MAX_UNREAD_COUNT);
    }

    ~BufferedFileInputStreamReader()
    {
        if (NULL != buffer)
        {
            delete[] buffer;
            buffer = NULL;
        }
        if (NULL != isEof)
        {
            delete[] isEof;
            isEof = NULL;
        }
    }

    int curChar()
    {
        if (!refill())
            return EOF;

        return isEof[bufferPos] ? EOF : buffer[bufferPos];
    }

    int nextChar()
    {
        if (!refill())
            return EOF;

        return isEof[bufferPos] ? EOF : buffer[bufferPos++];
    }

    void skipChar()
    {
        bufferPos++;
    }

    void unreadChar(int c)
    {   
        bufferPos--;
        if (bufferPos < 0)
            __testlib_fail("BufferedFileInputStreamReader::unreadChar(int): bufferPos < 0");
        isEof[bufferPos] = (c == EOF);
        buffer[bufferPos] = char(c == EOF ? EOFC : c);
    }

    std::string getName()
    {
        return name;
    }
    
    bool eof()
    {
        return !refill() || EOF == curChar();
    }
};

const size_t BufferedFileInputStreamReader::BUFFER_SIZE = 1000000;
const size_t BufferedFileInputStreamReader::MAX_UNREAD_COUNT = 100; 

/*
 * Streams to be used for reading data in checkers or validators.
 * Each read*() method moves pointer to the next character after the
 * read value.
 */
struct InStream
{
    /* Do not use them. */
    InStream();
    ~InStream();

    InputStreamReader* reader;

    std::FILE* file;
    std::string name;
    TMode mode;
    bool opened;
    bool stdfile;
    bool strict;

    int wordReserveSize;
    std::string _tmpReadToken;

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
    std::string readWord(const std::string& ptrn, const std::string& variableName = "");
    std::string readWord(const pattern& p, const std::string& variableName = "");
    /* The same as "readToken()", but ensures that token matches to given pattern. */
    std::string readToken(const std::string& ptrn, const std::string& variableName = "");
    std::string readToken(const pattern& p, const std::string& variableName = "");

    void readWordTo(std::string& result);
    void readWordTo(std::string& result, const pattern& p, const std::string& variableName = "");
    void readWordTo(std::string& result, const std::string& ptrn, const std::string& variableName = "");

    void readTokenTo(std::string& result);
    void readTokenTo(std::string& result, const pattern& p, const std::string& variableName = "");
    void readTokenTo(std::string& result, const std::string& ptrn, const std::string& variableName = "");

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
    long long readLong(long long minv, long long maxv, const std::string& variableName = "");
    /* As "readInteger()" but ensures that value in the range [minv,maxv]. */
    int readInteger(int minv, int maxv, const std::string& variableName = "");
    /* As "readInt()" but ensures that value in the range [minv,maxv]. */
    int readInt(int minv, int maxv, const std::string& variableName = "");

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
    double readReal(double minv, double maxv, const std::string& variableName = "");
    /* As "readDouble()" but ensures that value in the range [minv,maxv]. */
    double readDouble(double minv, double maxv, const std::string& variableName = "");
    
    /* 
     * As "readReal()" but ensures that value in the range [minv,maxv] and
     * number of digit after the decimal point is in range [minAfterPointDigitCount,maxAfterPointDigitCount]
     * and number is in the form "[-]digit(s)[.digit(s)]".
     */
    double readStrictReal(double minv, double maxv,
            int minAfterPointDigitCount, int maxAfterPointDigitCount,
            const std::string& variableName = "");
    /* 
     * As "readDouble()" but ensures that value in the range [minv,maxv] and
     * number of digit after the decimal point is in range [minAfterPointDigitCount,maxAfterPointDigitCount]
     * and number is in the form "[-]digit(s)[.digit(s)]".
     */
    double readStrictDouble(double minv, double maxv,
            int minAfterPointDigitCount, int maxAfterPointDigitCount,
            const std::string& variableName = "");
    
    /* As readLine(). */
    std::string readString();
    /* See readLine(). */
    void readStringTo(std::string& result);
    /* The same as "readLine()/readString()", but ensures that line matches to the given pattern. */
    std::string readString(const pattern& p, const std::string& variableName = "");
    /* The same as "readLine()/readString()", but ensures that line matches to the given pattern. */
    std::string readString(const std::string& ptrn, const std::string& variableName = "");
    /* The same as "readLine()/readString()", but ensures that line matches to the given pattern. */
    void readStringTo(std::string& result, const pattern& p, const std::string& variableName = "");
    /* The same as "readLine()/readString()", but ensures that line matches to the given pattern. */
    void readStringTo(std::string& result, const std::string& ptrn, const std::string& variableName = "");

    /* 
     * Reads line from the current position to EOLN or EOF. Moves stream pointer to 
     * the first character of the new line (if possible). 
     */
    std::string readLine();
    /* See readLine(). */
    void readLineTo(std::string& result);
    /* The same as "readLine()", but ensures that line matches to the given pattern. */
    std::string readLine(const pattern& p, const std::string& variableName = "");
    /* The same as "readLine()", but ensures that line matches to the given pattern. */
    std::string readLine(const std::string& ptrn, const std::string& variableName = "");
    /* The same as "readLine()", but ensures that line matches to the given pattern. */
    void readLineTo(std::string& result, const pattern& p, const std::string& variableName = "");
    /* The same as "readLine()", but ensures that line matches to the given pattern. */
    void readLineTo(std::string& result, const std::string& ptrn, const std::string& variableName = "");

    /* Reads EOLN or fails. Use it in validators. Calls "eoln()" method internally. */
    void readEoln();
    /* Reads EOF or fails. Use it in validators. Calls "eof()" method internally. */
    void readEof();

    /* 
     * Quit-functions aborts program with <result> and <message>:
     * input/answer streams replaces any result to FAIL.
     */
    NORETURN void quit(TResult result, const char* msg);
    /* 
     * Quit-functions aborts program with <result> and <message>:
     * input/answer streams replaces any result to FAIL.
     */
    NORETURN void quitf(TResult result, const char* msg, ...);
    /* 
     * Quit-functions aborts program with <result> and <message>:
     * input/answer streams replaces any result to FAIL.
     */
    NORETURN void quits(TResult result, std::string msg);

    void close();

    const static WORD LightGray = 0x07;    
    const static WORD LightRed = 0x0c;    
    const static WORD LightCyan = 0x0b;    
    const static WORD LightGreen = 0x0a;    
    const static WORD LightYellow = 0x0e;    

    static void textColor(WORD color);
    static void quitscr(WORD color, const char* msg);
    static void quitscrS(WORD color, std::string msg);
    void xmlSafeWrite(std::FILE * file, const char* msg);

private:
    InStream(const InStream&);
    InStream& operator =(const InStream&);
};

InStream inf;
InStream ouf;
InStream ans;
bool appesMode;
std::string resultName;
std::string checkerName = "untitled checker";
random_t rnd;
TTestlibMode testlibMode = _unknown;
double __testlib_points = std::numeric_limits<float>::infinity();

struct TestlibFinalizeGuard
{
    static bool alive;
    int quitCount, readEofCount;

    TestlibFinalizeGuard() : quitCount(0), readEofCount(0)
    {
        // No operations.
    }

    ~TestlibFinalizeGuard()
    {
        bool _alive = alive;
        alive = false;

        if (_alive)
        {
            if (testlibMode == _checker && quitCount == 0)
                __testlib_fail("Checker must end with quit or quitf call.");

            if (testlibMode == _validator && readEofCount == 0 && quitCount == 0)
                __testlib_fail("Validator must end with readEof call.");
        }
    }
};

bool TestlibFinalizeGuard::alive = true;
TestlibFinalizeGuard testlibFinalizeGuard;

/*
 * Call it to disable checks on finalization.
 */
void disableFinalizeGuard()
{
    TestlibFinalizeGuard::alive = false;
}

/* Interactor streams.
 */
std::fstream tout;

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

template <typename T>
static std::string toString(const T& t)
{
    return vtos(t);
}

InStream::InStream()
{
    file = NULL;
    name = "";
    mode = _input;
    strict = false;
    stdfile = false;
    wordReserveSize = 4;
}

InStream::~InStream()
{
    if (NULL != reader)
    {
        delete reader;
        reader = NULL;
    }
}

#ifdef __GNUC__
__attribute__((const))
#endif
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
    if (r == _points)
        return POINTS_EXIT_CODE;
    if (r == _unexpected_eof)
#ifdef ENABLE_UNEXPECTED_EOF
        return UNEXPECTED_EOF_EXIT_CODE;
#else
        return PE_EXIT_CODE;
#endif
    if (r >= _partially)
        return PC_BASE_EXIT_CODE + (r - _partially);
    return FAIL_EXIT_CODE;
}

void InStream::textColor(WORD color)
{
#if defined(ON_WINDOWS) && (!defined(_MSC_VER) || _MSC_VER>1400)
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, color);
#endif
}


NORETURN void halt(int exitCode)
{
#ifdef FOOTER
    InStream::textColor(InStream::LightGray);
    std::fprintf(stderr, "Checker: \"%s\"\n", checkerName.c_str());
    std::fprintf(stderr, "Exit code: %d\n", exitCode);
    InStream::textColor(InStream::LightGray);
#endif
    std::exit(exitCode);
}

NORETURN void InStream::quit(TResult result, const char* msg)
{
    if (TestlibFinalizeGuard::alive)
        testlibFinalizeGuard.quitCount++;

    if (mode != _output && result != _fail)
        quits(_fail, std::string(msg) + " (" + name + ")");

    std::FILE * resultFile;
    std::string errorName;
    
    if (result == _ok)
    {
        if (testlibMode != _interactor && !ouf.seekEof())
            quit(_dirt, "Extra information in the output file");
    }

    int pctype = result - _partially;
    bool isPartial = false;

    switch (result)
    {
    case _ok:
        errorName = "ok ";
        quitscrS(LightGreen, errorName);
        break;
    case _wa:
        errorName = "wrong answer ";
        quitscrS(LightRed, errorName);
        break;
    case _pe:
        errorName = "wrong output format ";
        quitscrS(LightRed, errorName);
        break;
    case _fail:
        errorName = "FAIL ";
        quitscrS(LightRed, errorName);
        break;
    case _dirt:
        errorName = "wrong output format ";
        quitscrS(LightCyan, errorName);
        result = _pe;
        break;
    case _points:
        errorName = "points ";
        quitscrS(LightYellow, errorName);
        break;
    case _unexpected_eof:
#ifdef ENABLE_UNEXPECTED_EOF
        errorName = "unexpected eof ";
#else
        errorName = "wrong output format ";
#endif
        quitscrS(LightCyan, errorName);
        break;
    default:
        if (result >= _partially)
        {
            char message[INLINE_CHAR_BUFFER_SIZE];
            std::sprintf(message, "partially correct (%d) ", pctype);
            errorName = std::string(message);
            isPartial = true;
            quitscrS(LightYellow, errorName);
        }
        else
            quit(_fail, "What is the code ??? ");
    }

    if (resultName != "")
    {
        resultFile = std::fopen(resultName.c_str(), "w");
        if (resultFile == NULL)
            quit(_fail, "Can not write to the result file");
        if (appesMode)
        {
            fprintf(resultFile, "<?xml version=\"1.0\" encoding=\"windows-1251\"?>");
            if (isPartial)
                fprintf(resultFile, "<result outcome = \"%s\" pctype = \"%d\">", outcomes[(int)_partially].c_str(), pctype);
            else
            {
                if (result != _points)
                    fprintf(resultFile, "<result outcome = \"%s\">", outcomes[(int)result].c_str());
                else
                {
                    if (__testlib_points == std::numeric_limits<float>::infinity())
                        quit(_fail, "Expected points, but infinity found");
                    fprintf(resultFile, "<result outcome = \"%s\" points = \"%.10f\">", outcomes[(int)result].c_str(), __testlib_points);
                }
            }
            xmlSafeWrite(resultFile, msg);
            fprintf(resultFile, "</result>\n");
        }
        else
             fprintf(resultFile, "%s", msg);
        if (NULL == resultFile || fclose(resultFile) != 0)
            quit(_fail, "Can not write to the result file");
    }

    quitscr(LightGray, msg);
    std::fprintf(stderr, "\n");

    if (inf.file)
        fclose(inf.file);
    if (ouf.file)
        fclose(ouf.file);
    if (ans.file)
        fclose(ans.file);
    if (tout.is_open())
        tout.close();

    textColor(LightGray);

    if (resultName != "")
        std::fprintf(stderr, "See file to check exit message\n");

    halt(resultExitCode(result));
}

#ifdef __GNUC__
    __attribute__ ((format (printf, 3, 4)))
#endif
NORETURN void InStream::quitf(TResult result, const char* msg, ...)
{
    FMT_TO_RESULT(msg, msg, message);
    InStream::quit(result, message.c_str());
}

NORETURN void InStream::quits(TResult result, std::string msg)
{
    InStream::quit(result, msg.c_str());
}

void InStream::xmlSafeWrite(std::FILE * file, const char* msg)
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

void InStream::quitscr(WORD color, const char* msg)
{
    if (resultName == "")
    {
        textColor(color);
        std::fprintf(stderr, "%s", msg);
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

    __testlib_set_binary(file);

    if (stdfile)
        reader = new FileInputStreamReader(file, name);
    else
        reader = new BufferedFileInputStreamReader(file, name);
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
    int c = reader->curChar();
    return char(c != EOF ? c : EOFC);
}

char InStream::nextChar()
{
    int c = reader->nextChar();
    return char(c != EOF ? c : EOFC);
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
    reader->unreadChar(c);
}

void InStream::skipChar()
{
    reader->skipChar();
}

void InStream::skipBlanks()
{
    while (isBlanks(reader->curChar()))
        reader->skipChar();
}

std::string InStream::readWord()
{
    readWordTo(_tmpReadToken);
    return _tmpReadToken;
}

void InStream::readWordTo(std::string& result)
{
    if (!strict)
        skipBlanks();

    int cur = reader->nextChar();

    if (cur == EOF)
        quit(_unexpected_eof, "Unexpected end of file - token expected");

    if (isBlanks(cur))
        quit(_pe, "Unexpected white-space - token expected");

    result.clear();

    while (!(isBlanks(cur) || cur == EOF))
    {
        result += char(cur);
        cur = reader->nextChar();
    }

    reader->unreadChar(cur);

    if (result.length() == 0)
        quit(_unexpected_eof, "Unexpected end of file or white-space - token expected");
}

std::string InStream::readToken()
{
    return readWord();
}

void InStream::readTokenTo(std::string& result)
{
    readWordTo(result);
}

static std::string __testlib_part(const std::string& s)
{
    if (s.length() <= 64)
        return s;
    else
        return s.substr(0, 30) + "..." + s.substr(s.length() - 31, 31);
}

std::string InStream::readWord(const pattern& p, const std::string& variableName)
{
    readWordTo(_tmpReadToken);
    if (!p.matches(_tmpReadToken))
    {
        if (variableName.empty())
            quit(_wa, ("Token \"" + __testlib_part(_tmpReadToken) + "\" doesn't correspond to pattern \"" + p.src() + "\"").c_str());
        else
            quit(_wa, ("Token parameter [name=" + variableName + "] equals to \"" + __testlib_part(_tmpReadToken) + "\", doesn't correspond to pattern \"" + p.src() + "\"").c_str());
    }
    return _tmpReadToken;
}

std::string InStream::readWord(const std::string& ptrn, const std::string& variableName)
{
    return readWord(pattern(ptrn), variableName);
}

std::string InStream::readToken(const pattern& p, const std::string& variableName)
{
    return readWord(p, variableName);
}

std::string InStream::readToken(const std::string& ptrn, const std::string& variableName)
{
    return readWord(ptrn, variableName);
}

void InStream::readWordTo(std::string& result, const pattern& p, const std::string& variableName)
{
    readWordTo(result);
    if (!p.matches(result))
    {
        if (variableName.empty())
            quit(_wa, ("Token \"" + __testlib_part(result) + "\" doesn't correspond to pattern \"" + p.src() + "\"").c_str());
        else
            quit(_wa, ("Token parameter [name=" + variableName + "] equals to \"" + __testlib_part(result) + "\", doesn't correspond to pattern \"" + p.src() + "\"").c_str());
    }
}

void InStream::readWordTo(std::string& result, const std::string& ptrn, const std::string& variableName)
{
    return readWordTo(result, pattern(ptrn), variableName);
}

void InStream::readTokenTo(std::string& result, const pattern& p, const std::string& variableName)
{
    return readWordTo(result, p, variableName);
}

void InStream::readTokenTo(std::string& result, const std::string& ptrn, const std::string& variableName)
{
    return readWordTo(result, ptrn, variableName);
}

#ifdef __GNUC__
__attribute__((pure))
#endif
static inline bool equals(long long integer, const char* s)
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
        int digit = int(integer % 10);

        if (s[length - 1] != '0' + digit)
            return false;

        length--;
        integer /= 10;
    }

    return length == 0;
}

static inline double stringToDouble(InStream& in, const char* buffer)
{
    double retval;

    size_t length = strlen(buffer);

    int minusCount = 0;
    int plusCount = 0;
    int decimalPointCount = 0;
    int digitCount = 0;
    int eCount = 0;

    for (size_t i = 0; i < length; i++)
    {
        if (('0' <= buffer[i] && buffer[i] <= '9') || buffer[i] == '.'
                || buffer[i] == 'e' || buffer[i] == 'E'
                || buffer[i] == '-' || buffer[i] == '+')
        {
            if ('0' <= buffer[i] && buffer[i] <= '9')
                digitCount++;
            if (buffer[i] == 'e' || buffer[i] == 'E')
                eCount++;
            if (buffer[i] == '-')
                minusCount++;
            if (buffer[i] == '+')
                plusCount++;
            if (buffer[i] == '.')
                decimalPointCount++;
        }
        else
            in.quit(_pe, ("Expected double, but \"" + __testlib_part(buffer) + "\" found").c_str());
    }

    // If for sure is not a number in standard notation or in e-notation.
    if (digitCount == 0 || minusCount > 2 || plusCount > 2 || decimalPointCount > 1 || eCount > 1)
        in.quit(_pe, ("Expected double, but \"" + __testlib_part(buffer) + "\" found").c_str());

    char* suffix = new char[length + 1];
    int scanned = std::sscanf(buffer, "%lf%s", &retval, suffix);
    bool empty = strlen(suffix) == 0;
    delete[] suffix;

    if (scanned == 1 || (scanned == 2 && empty))
    {
        if (__testlib_isNaN(retval) || __testlib_isInfinite(retval))
            in.quit(_pe, ("Expected double, but \"" + __testlib_part(buffer) + "\" found").c_str());
        return retval;
    }
    else
        in.quit(_pe, ("Expected double, but \"" + __testlib_part(buffer) + "\" found").c_str());
}

static inline double stringToStrictDouble(InStream& in, const char* buffer, int minAfterPointDigitCount, int maxAfterPointDigitCount)
{
    if (minAfterPointDigitCount < 0)
        in.quit(_fail, "stringToStrictDouble: minAfterPointDigitCount should be non-negative.");
    
    if (minAfterPointDigitCount > maxAfterPointDigitCount)
        in.quit(_fail, "stringToStrictDouble: minAfterPointDigitCount should be less or equal to maxAfterPointDigitCount.");

    double retval;

    size_t length = strlen(buffer);

    if (length == 0 || length > 1000)
        in.quit(_pe, ("Expected strict double, but \"" + __testlib_part(buffer) + "\" found").c_str());

    if (buffer[0] != '-' && (buffer[0] < '0' || buffer[0] > '9'))
        in.quit(_pe, ("Expected strict double, but \"" + __testlib_part(buffer) + "\" found").c_str());

    int pointPos = -1; 
    for (size_t i = 1; i + 1 < length; i++)
    {
        if (buffer[i] == '.')
        {
            if (pointPos > -1)
                in.quit(_pe, ("Expected strict double, but \"" + __testlib_part(buffer) + "\" found").c_str());
            pointPos = int(i);
        }
        if (buffer[i] != '.' && (buffer[i] < '0' || buffer[i] > '9'))
            in.quit(_pe, ("Expected strict double, but \"" + __testlib_part(buffer) + "\" found").c_str());
    }

    if (buffer[length - 1] < '0' || buffer[length - 1] > '9')
        in.quit(_pe, ("Expected strict double, but \"" + __testlib_part(buffer) + "\" found").c_str());

    int afterDigitsCount = (pointPos == -1 ? 0 : int(length) - pointPos - 1);
    if (afterDigitsCount < minAfterPointDigitCount || afterDigitsCount > maxAfterPointDigitCount)
        in.quit(_pe, ("Expected strict double with number of digits after point in range ["
            + vtos(minAfterPointDigitCount)
            + ","
            + vtos(maxAfterPointDigitCount)
            + "], but \"" + __testlib_part(buffer) + "\" found").c_str()
        );

    int firstDigitPos = -1;
    for (size_t i = 0; i < length; i++)
        if (buffer[i] >= '0' && buffer[i] <= '9')
        {
            firstDigitPos = int(i);
            break;
        }

    if (firstDigitPos > 1 || firstDigitPos == -1) 
        in.quit(_pe, ("Expected strict double, but \"" + __testlib_part(buffer) + "\" found").c_str());

    if (buffer[firstDigitPos] == '0' && firstDigitPos + 1 < int(length)
            && buffer[firstDigitPos + 1] >= '0' && buffer[firstDigitPos + 1] <= '9')
        in.quit(_pe, ("Expected strict double, but \"" + __testlib_part(buffer) + "\" found").c_str());

    char* suffix = new char[length + 1];
    int scanned = std::sscanf(buffer, "%lf%s", &retval, suffix);
    bool empty = strlen(suffix) == 0;
    delete[] suffix;

    if (scanned == 1 || (scanned == 2 && empty))
    {
        if (__testlib_isNaN(retval) || __testlib_isInfinite(retval))
            in.quit(_pe, ("Expected double, but \"" + __testlib_part(buffer) + "\" found").c_str());
        return retval;
    }
    else
        in.quit(_pe, ("Expected double, but \"" + __testlib_part(buffer) + "\" found").c_str());
}

static inline long long stringToLongLong(InStream& in, const char* buffer)
{
    if (strcmp(buffer, "-9223372036854775808") == 0)
        return LLONG_MIN;

    bool minus = false;
    size_t length = strlen(buffer);
    
    if (length > 1 && buffer[0] == '-')
        minus = true;

    if (length > 20)
        in.quit(_pe, ("Expected integer, but \"" + __testlib_part(buffer) + "\" found").c_str());

    long long retval = 0LL;

    int zeroes = 0;
    int processingZeroes = true;
    
    for (int i = (minus ? 1 : 0); i < int(length); i++)
    {
        if (buffer[i] == '0' && processingZeroes)
            zeroes++;
        else
            processingZeroes = false;

        if (buffer[i] < '0' || buffer[i] > '9')
            in.quit(_pe, ("Expected integer, but \"" + __testlib_part(buffer) + "\" found").c_str());
        retval = retval * 10 + (buffer[i] - '0');
    }

    if (retval < 0)
        in.quit(_pe, ("Expected integer, but \"" + __testlib_part(buffer) + "\" found").c_str());
    
    if ((zeroes > 0 && (retval != 0 || minus)) || zeroes > 1)
        in.quit(_pe, ("Expected integer, but \"" + __testlib_part(buffer) + "\" found").c_str());

    retval = (minus ? -retval : +retval);

    if (length < 19)
        return retval;

    if (equals(retval, buffer))
        return retval;
    else
        in.quit(_pe, ("Expected int64, but \"" + __testlib_part(buffer) + "\" found").c_str());
}

int InStream::readInteger()
{
    if (!strict && seekEof())
        quit(_unexpected_eof, "Unexpected end of file - int32 expected");

    readWordTo(_tmpReadToken);
    
    long long value = stringToLongLong(*this, _tmpReadToken.c_str());
    if (value < INT_MIN || value > INT_MAX)
        quit(_pe, ("Expected int32, but \"" + __testlib_part(_tmpReadToken) + "\" found").c_str());
    
    return int(value);
}

long long InStream::readLong()
{
    if (!strict && seekEof())
        quit(_unexpected_eof, "Unexpected end of file - int64 expected");

    readWordTo(_tmpReadToken);
    return stringToLongLong(*this, _tmpReadToken.c_str());
}

long long InStream::readLong(long long minv, long long maxv, const std::string& variableName)
{
    long long result = readLong();

    if (result < minv || result > maxv)
    {
        if (variableName.empty())
            quit(_wa, ("Integer " + vtos(result) + " violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());
        else
            quit(_wa, ("Integer parameter [name=" + variableName + "] equals to " + vtos(result) + ", violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());
    }

    return result;
}

int InStream::readInt()
{
    return readInteger();
}

int InStream::readInt(int minv, int maxv, const std::string& variableName)
{
    int result = readInt();

    if (result < minv || result > maxv)
    {
        if (variableName.empty())
            quit(_wa, ("Integer " + vtos(result) + " violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());
        else
            quit(_wa, ("Integer parameter [name=" + std::string(variableName) + "] equals to " + vtos(result) + ", violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());
    }

    return result;
}

int InStream::readInteger(int minv, int maxv, const std::string& variableName)
{
    return readInt(minv, maxv, variableName);
}

double InStream::readReal()
{
    if (!strict && seekEof())
        quit(_unexpected_eof, "Unexpected end of file - double expected");

    return stringToDouble(*this, readWord().c_str());
}

double InStream::readDouble()
{
    return readReal();
}

double InStream::readReal(double minv, double maxv, const std::string& variableName)
{
    double result = readReal();

    if (result < minv || result > maxv)
    {
        if (variableName.empty())
            quit(_wa, ("Double " + vtos(result) + " violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());
        else
            quit(_wa, ("Double parameter [name=" + variableName + "] equals to " + vtos(result) + ", violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());
    }

    return result;
}

double InStream::readDouble(double minv, double maxv, const std::string& variableName)
{
    return readReal(minv, maxv, variableName);
}                                           

double InStream::readStrictReal(double minv, double maxv,
        int minAfterPointDigitCount, int maxAfterPointDigitCount,
        const std::string& variableName)
{
    if (!strict && seekEof())
        quit(_unexpected_eof, "Unexpected end of file - strict double expected");

    double result = stringToStrictDouble(*this, readWord().c_str(),
            minAfterPointDigitCount, maxAfterPointDigitCount);

    if (result < minv || result > maxv)
    {
        if (variableName.empty())
            quit(_wa, ("Strict double " + vtos(result) + " violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());
        else
            quit(_wa, ("Strict double parameter [name=" + variableName + "] equals to " + vtos(result) + ", violates the range [" + vtos(minv) + ", " + vtos(maxv) + "]").c_str());
    }

    return result;
}

double InStream::readStrictDouble(double minv, double maxv,
        int minAfterPointDigitCount, int maxAfterPointDigitCount,
        const std::string& variableName)
{
    return readStrictReal(minv, maxv,
            minAfterPointDigitCount, maxAfterPointDigitCount,
            variableName);
}

bool InStream::eof()
{
    if (!strict && NULL == file)
        return true;

    return reader->eof();
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

    int c = reader->nextChar();

    if (!strict)
    {
        if (c == EOF)
            return true;

        if (c == CR)
        {
            c = reader->nextChar();

            if (c != LF)
            {
                reader->unreadChar(CR);
                reader->unreadChar(c);
                return false;
            }
            else
                return true;
        }
        
        if (c == LF)
            return true;

        reader->unreadChar(c);
        return false;
    }
    else
    {
        bool returnCr = false;

#ifdef ON_WINDOWS
        if (c != CR)
        {
            reader->unreadChar(c);
            return false;
        }
        else
        {
            if (!returnCr)
                returnCr = true;
            c = reader->nextChar();
        }
#endif        
        if (c != LF)
        {
            if (returnCr)
                reader->unreadChar(CR);
            reader->unreadChar(LF);
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

    if (TestlibFinalizeGuard::alive && this == &inf)
        testlibFinalizeGuard.readEofCount++;
}

bool InStream::seekEoln()
{
    if (NULL == file)
        return true;
    
    int cur;
    do
    {
        cur = reader->nextChar();
    } 
    while (cur == SPACE || cur == TAB);

    reader->unreadChar(cur);
    return eoln();
}

void InStream::nextLine()
{
    readLine();
}

void InStream::readStringTo(std::string& result)
{
    if (NULL == file)
        quit(_pe, "Expected line");

    result.clear();
    int cur;

    for (;;)
    {
        cur = reader->curChar();

        if (isEoln(cur))
            break;

        if (cur == EOF)
            break;

        result += char(reader->nextChar());
    }

    if (strict)
        readEoln();
    else
        eoln();
}

std::string InStream::readString()
{
    readStringTo(_tmpReadToken);
    return _tmpReadToken;
}

void InStream::readStringTo(std::string& result, const pattern& p, const std::string& variableName)
{
    readStringTo(result);
    if (!p.matches(result))
    {
        if (variableName.empty())
            quit(_wa, ("Line \"" + __testlib_part(result) + "\" doesn't correspond to pattern \"" + p.src() + "\"").c_str());
        else
            quit(_wa, ("Line [name=" + variableName + "] equals to \"" + __testlib_part(result) + "\", doesn't correspond to pattern \"" + p.src() + "\"").c_str());
    }
}

void InStream::readStringTo(std::string& result, const std::string& ptrn, const std::string& variableName)
{
    readStringTo(result, pattern(ptrn), variableName);
}

std::string InStream::readString(const pattern& p, const std::string& variableName)
{
    readStringTo(_tmpReadToken, p, variableName);
    return _tmpReadToken;
}

std::string InStream::readString(const std::string& ptrn, const std::string& variableName)
{
    readStringTo(_tmpReadToken, ptrn, variableName);
    return _tmpReadToken;
}

void InStream::readLineTo(std::string& result)
{
    readStringTo(result);
}

std::string InStream::readLine()
{
    return readString();
}

void InStream::readLineTo(std::string& result, const pattern& p, const std::string& variableName)
{
    readStringTo(result, p, variableName);
}

void InStream::readLineTo(std::string& result, const std::string& ptrn, const std::string& variableName)
{
    readStringTo(result, ptrn, variableName);
}

std::string InStream::readLine(const pattern& p, const std::string& variableName)
{
    return readString(p, variableName);
}

std::string InStream::readLine(const std::string& ptrn, const std::string& variableName)
{
    return readString(ptrn, variableName);
}

void InStream::close()
{
    if (opened)
        fclose(file);
    
    if (NULL != reader)
    {
        delete reader;
        reader = NULL;
    }
    
    opened = false;
}

NORETURN void quit(TResult result, const std::string& msg)
{
    ouf.quit(result, msg.c_str());
}

NORETURN void quit(TResult result, const char* msg)
{
    ouf.quit(result, msg);
}

NORETURN void __testlib_quitp(double points, const char* message)
{
    __testlib_points = points;
    char buffer[INLINE_CHAR_BUFFER_SIZE];
    if (NULL == message || 0 == strlen(message))
        std::sprintf(buffer, "%.10f", points);
    else
        std::sprintf(buffer, "%.10f %s", points, message);
    quit(_points, buffer);
}

NORETURN void quitp(float points, const std::string& message = "")
{
    __testlib_quitp(double(points), message.c_str());
}

NORETURN void quitp(double points, const std::string& message = "")
{
    __testlib_quitp(points, message.c_str());
}

NORETURN void quitp(long double points, const std::string& message = "")
{
    __testlib_quitp(double(points), message.c_str());
}

template<typename F>
#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
NORETURN void quitp(F points, const char* format, ...)
{
    FMT_TO_RESULT(format, format, message);
    quitp(points, message);
}

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
NORETURN void quitf(TResult result, const char* format, ...)
{
    FMT_TO_RESULT(format, format, message);
    quit(result, message);
}

#ifdef __GNUC__
__attribute__ ((format (printf, 3, 4)))
#endif
void quitif(bool condition, TResult result, const char* format, ...)
{
    if (condition)
    {
        FMT_TO_RESULT(format, format, message);
        quit(result, message);
    }
}

NORETURN void __testlib_help()
{
    InStream::textColor(InStream::LightCyan);
    std::fprintf(stderr, "TESTLIB %s, http://code.google.com/p/testlib/ ", VERSION);
    std::fprintf(stderr, "by Mike Mirzayanov, copyright(c) 2005-2013\n");
    std::fprintf(stderr, "Checker name: \"%s\"\n", checkerName.c_str());
    InStream::textColor(InStream::LightGray);

    std::fprintf(stderr, "\n");
    std::fprintf(stderr, "Latest features: \n");
    for (size_t i = 0; i < sizeof(latestFeatures) / sizeof(char*); i++)
    {
        std::fprintf(stderr, "*) %s\n", latestFeatures[i]);
    }
    std::fprintf(stderr, "\n");

    std::fprintf(stderr, "Program must be run with the following arguments: \n");
    std::fprintf(stderr, "    <input-file> <output-file> <answer-file> [<report-file> [<-appes>]]\n\n");

    std::exit(0);
}

static void __testlib_ensuresPreconditions()
{
    // testlib assumes: sizeof(int) = 4.
    __TESTLIB_STATIC_ASSERT(sizeof(int) == 4);

    // testlib assumes: INT_MAX == 2147483647.
    __TESTLIB_STATIC_ASSERT(INT_MAX == 2147483647);

    // testlib assumes: sizeof(long long) = 8.
    __TESTLIB_STATIC_ASSERT(sizeof(long long) == 8);

    // testlib assumes: no -ffast-math.
    if (!__testlib_isNaN(+__testlib_nan()))
        quit(_fail, "Function __testlib_isNaN is not working correctly: possible reason is '-ffast-math'");
    if (!__testlib_isNaN(-__testlib_nan()))
        quit(_fail, "Function __testlib_isNaN is not working correctly: possible reason is '-ffast-math'");
}

void registerGen(int argc, char* argv[], int randomGeneratorVersion)
{
    if (randomGeneratorVersion < 0 || randomGeneratorVersion > 1)
        quitf(_fail, "Random generator version is expected to be 0 or 1.");
    random_t::version = randomGeneratorVersion;

    __testlib_ensuresPreconditions();

    testlibMode = _generator;
    __testlib_set_binary(stdin);
    rnd.setSeed(argc, argv);
}

#ifdef USE_RND_AS_BEFORE_087
void registerGen(int argc, char* argv[])
{
    registerGen(argc, argv, 0);
}
#else
    template <class T>
    class use_registerGen_argc_argv_0_or_registerGen_argc_argv_1 {
    private:
        ~use_registerGen_argc_argv_0_or_registerGen_argc_argv_1() {
        }
    };

    template <class T>
#ifdef __GNUC__
    __attribute__ ((deprecated("Use registerGen(argc, argv, 0) or registerGen(argc, argv, 1)."
            " The third parameter stands for the random generator version."
            " If you are trying to compile old generator use macro -DUSE_RND_AS_BEFORE_087 or registerGen(argc, argv, 0)."
            " Version 1 has been released on Spring, 2013. Use it to write new generators.")))
#endif
#ifdef _MSC_VER
    __declspec(deprecated("Use registerGen(argc, argv, 0) or registerGen(argc, argv, 1)."
            " The third parameter stands for the random generator version."
            " If you are trying to compile old generator use macro -DUSE_RND_AS_BEFORE_087 or registerGen(argc, argv, 0)."
            " Version 1 has been released on Spring, 2013. Use it to write new generators."))
#endif
    void registerGen(T argc, char* argv[])
    {
        use_registerGen_argc_argv_0_or_registerGen_argc_argv_1<void> use_registerGen_argc_argv_0_or_registerGen_argc_argv_1;
    }
#endif

void registerInteraction(int argc, char* argv[])
{
    __testlib_ensuresPreconditions();

    testlibMode = _interactor;
    __testlib_set_binary(stdin);

    if (argc > 1 && !strcmp("--help", argv[1]))
        __testlib_help();
    
    if (argc < 3 || argc > 6)
    {
        quit(_fail, std::string("Program must be run with the following arguments: ") +
            std::string("<input-file> <output-file> [<answer-file> [<report-file> [<-appes>]]]") + 
            "\nUse \"--help\" to get help information");
    }

    if (argc <= 4)
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

    if (setvbuf(stdin,  NULL, _IONBF, 0) != 0)
        quit(_fail, std::string("Can not disable buffer for stdin"));

    if (setvbuf(stdout,  NULL, _IONBF, 0) != 0)
        quit(_fail, std::string("Can not disable buffer for stdout"));
    
    if (setvbuf(stderr,  NULL, _IONBF, 0) != 0)
        quit(_fail, std::string("Can not disable buffer for stderr"));

    inf.init(argv[1], _input);

    tout.open(argv[2], std::ios_base::out);
    if (tout.fail() || !tout.is_open())
        quit(_fail, std::string("Can not write to the test-output-file '") + argv[2] + std::string("'"));

    ouf.init(stdin, _output);
    
    if (argc >= 4)
        ans.init(argv[3], _answer);
}

void registerValidation()
{
    __testlib_ensuresPreconditions();

    testlibMode = _validator;
    __testlib_set_binary(stdin);

    inf.init(stdin, _input);
    inf.strict = true;
}

void registerTestlibCmd(int argc, char* argv[])
{
    __testlib_ensuresPreconditions();

    testlibMode = _checker;
    __testlib_set_binary(stdin);    

    if (argc > 1 && !strcmp("--help", argv[1]))
        __testlib_help();

    if (argc < 4 || argc > 6)
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

    char** argv = new char*[argc + 1];
    
    va_list ap;
    va_start(ap, argc);
    argv[0] = NULL;
    for (int i = 0; i < argc; i++)
    {
        argv[i + 1] = va_arg(ap, char*);
    }
    va_end(ap);

    registerTestlibCmd(argc + 1, argv);
    delete[] argv;
}

#ifdef __GNUC__
__attribute__((const))
#endif
inline bool doubleCompare(double expected, double result, double MAX_DOUBLE_ERROR)
{
        if (__testlib_isNaN(expected))
        {
            return __testlib_isNaN(result);
        }
        else 
            if (__testlib_isInfinite(expected))
            {
                if (expected > 0)
                {
                    return result > 0 && __testlib_isInfinite(result);
                }
                else
                {
                    return result < 0 && __testlib_isInfinite(result);
                }
            }
            else 
                if (__testlib_isNaN(result) || __testlib_isInfinite(result))
                {
                    return false;
                }
                else 
                if (__testlib_abs(result - expected) <= MAX_DOUBLE_ERROR + 1E-15)
                {
                    return true;
                }
                else
                {
                    double minv = __testlib_min(expected * (1.0 - MAX_DOUBLE_ERROR),
                                 expected * (1.0 + MAX_DOUBLE_ERROR));
                    double maxv = __testlib_max(expected * (1.0 - MAX_DOUBLE_ERROR),
                                  expected * (1.0 + MAX_DOUBLE_ERROR));
                    return result + 1E-15 >= minv && result <= maxv + 1E-15;
                }
}

#ifdef __GNUC__
__attribute__((const))
#endif
inline double doubleDelta(double expected, double result)
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

static inline void __testlib_ensure(bool cond, const std::string& msg)
{
    if (!cond)
        quit(_fail, msg.c_str());
}

static inline void __testlib_ensure(bool cond, const char* msg)
{
    if (!cond)
        quit(_fail, msg);
}

#define ensure(cond) __testlib_ensure(cond, "Condition failed: \"" #cond "\"")

#ifdef __GNUC__
__attribute__ ((format (printf, 2, 3)))
#endif
inline void ensuref(bool cond, const char* format, ...)
{
    if (!cond)
    {
        FMT_TO_RESULT(format, format, message);
        __testlib_ensure(cond, message);
    }
}

NORETURN static void __testlib_fail(const std::string& message)
{
    quitf(_fail, "%s", message.c_str());
}

#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
void setName(const char* format, ...)
{
    FMT_TO_RESULT(format, format, name);
    checkerName = name;
}

/* 
 * Do not use random_shuffle, because it will produce different result
 * for different C++ compilers.
 *
 * This implementation uses testlib random_t to produce random numbers, so
 * it is stable.
 */ 
template<typename _RandomAccessIter>
void shuffle(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        std::iter_swap(__i, __first + rnd.next(int(__i - __first) + 1));
}


template<typename _RandomAccessIter>
#ifdef __GNUC__
__attribute__ ((error("Don't use random_shuffle(), use shuffle() instead")))
#endif
void random_shuffle(_RandomAccessIter , _RandomAccessIter )
{
    quitf(_fail, "Don't use random_shuffle(), use shuffle() instead");
}

#ifdef __GLIBC__
#  define RAND_THROW_STATEMENT throw()
#else
#  define RAND_THROW_STATEMENT
#endif

#ifdef __GNUC__
__attribute__ ((error("Don't use rand(), use rnd.next() instead")))
#endif
int rand() RAND_THROW_STATEMENT
{
    quitf(_fail, "Don't use rand(), use rnd.next() instead");
    
    /* This line never runs. */
    //throw "Don't use rand(), use rnd.next() instead";
}


#ifdef __GNUC__
__attribute__ ((error("Don't use srand(), you should use " 
        "'registerGen(argc, argv, 1);' to initialize generator seed "
        "by hash code of the command line params. The third parameter "
        "is randomGeneratorVersion (currently the latest is 1).")))
#endif
void srand(unsigned int seed) RAND_THROW_STATEMENT
{
    quitf(_fail, "Don't use srand(), you should use " 
        "'registerGen(argc, argv, 1);' to initialize generator seed "
        "by hash code of the command line params. The third parameter "
        "is randomGeneratorVersion (currently the latest is 1) [ignored seed=%d].", seed);
}

void startTest(int test)
{
    const std::string testFileName = vtos(test);
    if (NULL == freopen(testFileName.c_str(), "wt", stdout))
        __testlib_fail("Unable to write file '" + testFileName + "'");
}

#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
std::string format(const char* fmt, ...)
{
    FMT_TO_RESULT(fmt, fmt, result);
    return result;
}

std::string format(const std::string& fmt, ...)
{
    FMT_TO_RESULT(fmt, fmt.c_str(), result);
    return result;
}

inline std::string upperCase(std::string s)
{
    for (size_t i = 0; i < s.length(); i++)
        if ('a' <= s[i] && s[i] <= 'z')
            s[i] = char(s[i] - 'a' + 'A');
    return s;
}

inline std::string lowerCase(std::string s)
{
    for (size_t i = 0; i < s.length(); i++)
        if ('A' <= s[i] && s[i] <= 'Z')
            s[i] = char(s[i] - 'A' + 'a');
    return s;
}

inline std::string compress(const std::string& s)
{
    return __testlib_part(s);
}

inline std::string englishEnding(int x)
{
    x %= 100;
    if (x / 10 == 1)
        return "th";
    if (x % 10 == 1)
        return "st";
    if (x % 10 == 2)
        return "nd";
    if (x % 10 == 3)
        return "rd";
    return "th";
}

inline std::string trim(const std::string& s)
{
    if (s.empty())
        return s;

    int left = 0;
    while (left < int(s.length()) && isBlanks(s[left]))
        left++;
    if (left >= int(s.length()))
        return "";

    int right = int(s.length()) - 1;
    while (right >= 0 && isBlanks(s[right]))
        right--;
    if (right < 0)
        return "";

    return s.substr(left, right - left + 1);
}

template <typename _ForwardIterator, typename _Separator>
std::string join(_ForwardIterator first, _ForwardIterator last, _Separator separator)
{
    std::stringstream ss;
    bool repeated = false;
    for (_ForwardIterator i = first; i != last; i++)
    {
        if (repeated)
            ss << separator;
        else
            repeated = true;
        ss << *i;
    }
    return ss.str();
}

template <typename _ForwardIterator>
std::string join(_ForwardIterator first, _ForwardIterator last)
{
    return join(first, last, ' ');
}

template <typename _Collection, typename _Separator>
std::string join(const _Collection& collection, _Separator separator)
{
    return join(collection.begin(), collection.end(), separator);
}

template <typename _Collection>
std::string join(const _Collection& collection)
{
    return join(collection, ' ');
}

static std::string removeDoubleTrailingZeroes(std::string value)
{
    while (!value.empty() && value[value.length() - 1] == '0' && value.find('.') != std::string::npos)
        value = value.substr(0, value.length() - 1);
    value += '0';
    return value;
}

template <typename T>
void expectedButFound(TResult result, T expected, T found, const char* prependFormat = "", ...)
{
    FMT_TO_RESULT(prependFormat, prependFormat, prepend);
    std::string expectedString = vtos(expected);
    std::string foundString = vtos(found);
    expectedButFound(result, expectedString, foundString, prepend.c_str());
}

template <>
void expectedButFound<std::string>(TResult result, std::string expected, std::string found, const char* prependFormat, ...)
{
    FMT_TO_RESULT(prependFormat, prependFormat, prepend);
    std::string message;
    if (strlen(prependFormat) != 0)
        message = format("%s: expected '%s', but found '%s'",
            compress(prepend).c_str(), compress(expected).c_str(), compress(found).c_str());
    else
        message = format("expected '%s', but found '%s'",
            compress(expected).c_str(), compress(found).c_str());
    quitf(result, "%s", message.c_str());
}

template <>
void expectedButFound<double>(TResult result, double expected, double found, const char* prependFormat, ...)
{
    FMT_TO_RESULT(prependFormat, prependFormat, prepend);
    std::string expectedString = removeDoubleTrailingZeroes(format("%.12lf", expected));
    std::string foundString = removeDoubleTrailingZeroes(format("%.12lf", found));
    expectedButFound(result, expectedString, foundString, prepend.c_str());
}

template <>
void expectedButFound<const char*>(TResult result, const char* expected, const char* found, const char* prependFormat, ...)
{
    FMT_TO_RESULT(prependFormat, prependFormat, prepend);
    expectedButFound<std::string>(result, std::string(expected), std::string(found), prepend.c_str());
}

template <>
void expectedButFound<float>(TResult result, float expected, float found, const char* prependFormat, ...)
{
    FMT_TO_RESULT(prependFormat, prependFormat, prepend);
    expectedButFound<double>(result, double(expected), double(found), prepend.c_str());
}

template <>
void expectedButFound<long double>(TResult result, long double expected, long double found, const char* prependFormat, ...)
{
    FMT_TO_RESULT(prependFormat, prependFormat, prepend);
    expectedButFound<double>(result, double(expected), double(found), prepend.c_str());
}

#endif
