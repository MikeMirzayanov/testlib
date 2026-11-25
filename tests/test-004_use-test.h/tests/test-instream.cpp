TEST(instream) {
    {
        InStream s(inf, "this 100500 the test 123\n\n");
        ensure(s.readWord() == "this");
        ensure(s.readInt() == 100500);
        ensure(s.readToken() == "the");
        ensure(s.readLine() == " test 123");
        ensure(s.readLine() == "");
        ensure(s.eof());
        ensure_exit(3, [&](){s.readWord();});
    }
    {
        ensure_exit(3, [](){InStream(inf, "").readWord();});
    }
    {
        InStream s(inf, "3\n10 20 30\n");
        ensure(s.readInt(1, 3, "n") == 3);
        ensure(s.readInts(3, 10, 30, "a") == vector<int>({10, 20, 30}));
    }
    {
        ensure_exit(3, [](){InStream(inf, "1").readInt(0, 0, "n");});
        ensure_exit(3, [](){InStream(inf, "abacaba").readToken("[abc]{6}", "n");});
        ensure(InStream(inf, "abacaba").readToken("[abc]{7}", "s") == "abacaba");
        ensure(InStream(inf, "abacaba").readToken("a|test|abacaba|ok|abac", "s") == "abacaba");
        ensure_exit(3, [](){InStream(inf, "abacab").readToken("a|test|abacaba|ok|abac", "n");});
        ensure_exit(3, [](){InStream(inf, "abacabaa").readToken("a|test|abacaba|ok|abac", "n");});
        ensure_exit(3, [](){InStream(inf, "abacaba!").readToken("a|test|abacaba|ok|abac", "n");});
        InStream(inf, "1 2 3 4 1").readInts(5, 1, 4, "a");
        ensure_exit(3, [](){InStream(inf, "1 2 3 4 1").readInts(5, 1, 3, "a");});
        {
            string result;
            InStream(inf, "\t\t100500\t").readTokenTo(result, "[0-9]{1,6}", "n");
            ensure(result == "100500");
        }
    }
    {
        InStream s(inf, "\xEF\xBB\xBF" "content");
        s.skipBom();
        ensure(s.readWord() == "content");
        ensure(s.eof());
    }
    {
        InStream s(inf, "content");
        s.skipBom();
        ensure(s.readWord() == "content");
        ensure(s.eof());
    }
    {
        InStream s(inf, "\xEF" "content");
        s.skipBom();
        ensure(s.readChar() == '\xEF');
        ensure(s.readWord() == "content");
        ensure(s.eof());
    }
    {
        const char *unreadPath = "test-004-unread.tmp";
        const int bytes[] = {0x00, 0x7F, 0x80, 0xEF, 0xFE, 0xFF};
        for (size_t i = 0; i < sizeof(bytes) / sizeof(bytes[0]); i++) {
            int byte = bytes[i];
            std::string content;
            content += char(byte);
            content += 'Z';

            {
                InStream s(inf, content);
                char c = s.nextChar();
                s.unreadChar(c);
                ensure(!s.eof());
                ensure(!isEof(s.reader->curChar()));
                ensure(s.reader->curChar() == byte);
                ensure(s.curChar() == char(byte));
                ensure(s.readChar() == char(byte));
                ensure(s.readChar() == 'Z');
                ensure(s.eof());
                ensure(isEof(s.curChar()));
            }

            {
                std::FILE *out = std::fopen(unreadPath, "wb");
                ensure(out != NULL);
                ensure(std::fwrite(content.data(), 1, content.size(), out) == content.size());
                std::fclose(out);
            }

            {
                InStream s;
                s.init(unreadPath, _input);
                char c = s.nextChar();
                s.unreadChar(c);
                ensure(!s.eof());
                ensure(!isEof(s.reader->curChar()));
                ensure(s.reader->curChar() == byte);
                ensure(s.curChar() == char(byte));
                ensure(s.readChar() == char(byte));
                ensure(s.readChar() == 'Z');
                ensure(s.eof());
                ensure(isEof(s.curChar()));
            }

            {
                std::FILE *in = std::fopen(unreadPath, "rb");
                ensure(in != NULL);
                InStream s;
                s.stdfile = true;
                s.reset(in);
                char c = s.nextChar();
                s.unreadChar(c);
                ensure(!s.eof());
                ensure(!isEof(s.reader->curChar()));
                ensure(s.reader->curChar() == byte);
                ensure(s.curChar() == char(byte));
                ensure(s.readChar() == char(byte));
                ensure(s.readChar() == 'Z');
                ensure(s.eof());
                ensure(isEof(s.curChar()));
            }
        }

        for (int strictMode = 0; strictMode <= 1; strictMode++) {
#if (defined(ON_WINDOWS) && !defined(FOR_LINUX)) || defined(FOR_WINDOWS)
            const std::string eolnStr = strictMode ? "\r\n" : "\n";
#else
            const std::string eolnStr = "\n";
#endif
            std::string data;
            data += char(0xFF);
            data += "ab";
            data += eolnStr;
            data += "42";
            data += eolnStr;

            {
                std::FILE *out = std::fopen(unreadPath, "wb");
                ensure(out != NULL);
                ensure(std::fwrite(data.data(), 1, data.size(), out) == data.size());
                std::fclose(out);
            }

            for (int useStdFile = 0; useStdFile <= 1; useStdFile++) {
                InStream s;
                if (useStdFile) {
                    std::FILE *in = std::fopen(unreadPath, "rb");
                    ensure(in != NULL);
                    s.stdfile = true;
                    s.reset(in);
                } else {
                    s.init(unreadPath, _input);
                }
                s.strict = (strictMode != 0);

                char first = s.readChar();
                ensure(first == char(0xFF));
                s.unreadChar(first);
                std::string tok = s.readToken();
                ensure(tok.size() == 3 && tok[0] == char(0xFF) && tok[1] == 'a' && tok[2] == 'b');
                s.readEoln();
                char digit = s.readChar();
                ensure(digit == '4');
                s.unreadChar(digit);
                ensure(s.readInt() == 42);
                s.readEoln();
                ensure(s.eof());
            }
        }

        // Characterize unreadChar after reading at actual EOF:
        // StringInputStreamReader and FileInputStreamReader remain at EOF, whereas
        // BufferedFileInputStreamReader inserts a 0xFF byte (documented limitation:
        // InStream::unreadChar(char) cannot put back EOF).
        {
            InStream s(inf, "");
            char c = s.nextChar();
            s.unreadChar(c);
            ensure(s.eof());
            ensure(s.reader->curChar() == EOFC);
        }
        {
            std::FILE *out = std::fopen(unreadPath, "wb");
            ensure(out != NULL);
            std::fclose(out);
        }
        {
            std::FILE *in = std::fopen(unreadPath, "rb");
            ensure(in != NULL);
            InStream s;
            s.stdfile = true;
            s.reset(in);
            char c = s.nextChar();
            s.unreadChar(c);
            ensure(s.eof());
            ensure(s.reader->curChar() == EOFC);
        }
        {
            InStream s;
            s.init(unreadPath, _input);
            char c = s.nextChar();
            s.unreadChar(c);
            ensure(!s.eof());
            ensure(s.reader->curChar() == 255);
            ensure(s.readChar() == char(255));
            ensure(s.eof());
        }

        std::remove(unreadPath);
    }
}

TEST(instream_init_sets_mode) {
    const char *path = "test-004-init.tmp";
    const char *missing = "test-004-missing.tmp";
    std::FILE *out = std::fopen(path, "wb");
    ensure(out != NULL);
    std::fputs("12 ab\n", out);
    std::fclose(out);

    const TMode modes[] = {_input, _output, _answer};
    for (size_t i = 0; i < sizeof(modes) / sizeof(modes[0]); i++) {
        {
            InStream s;
            s.init(path, modes[i]);
            ensure(s.mode == modes[i]);
            ensure(s.readInt() == 12);
        }
        {
            std::FILE *in = std::fopen(path, "rb");
            ensure(in != NULL);
            InStream s;
            s.init(in, modes[i]);
            ensure(s.mode == modes[i]);
            ensure(s.readToken() == "12");
        }
    }

    // The mode is set before the file is opened: a missing file is not an error for input,
    // a presentation error for output and a failure for answer.
    {
        InStream s;
        s.init(missing, _input);
        ensure(s.mode == _input);
    }
    ensure_exit(2, [&](){InStream s; s.init(missing, _output);});
    ensure_exit(3, [&](){InStream s; s.init(missing, _answer);});

    std::remove(path);
}

TEST(instream_reader_history) {
    struct ModeGuard {
        TTestlibMode saved;
        ModeGuard() : saved(testlibMode) {}
        ~ModeGuard() { testlibMode = saved; }
    } modeGuard;

    const char *historyPath = "test-004-history.tmp";

    // 1. Non-validator modes must not record readChars or undoReadChars.
    {
        const unsigned char raw[] = {'A', 0x00, 0x80, 0xFF, '\n', '4', '2', '\n'};
        std::FILE *out = std::fopen(historyPath, "wb");
        ensure(out != NULL);
        ensure(std::fwrite(raw, 1, sizeof(raw), out) == sizeof(raw));
        std::fclose(out);

        const TTestlibMode nonValidatorModes[] = {_unknown, _checker, _generator, _interactor, _scorer};
        const int highBytes[] = {0x00, 0x80, 0xFF};

        for (size_t mi = 0; mi < sizeof(nonValidatorModes) / sizeof(nonValidatorModes[0]); mi++) {
            testlibMode = nonValidatorModes[mi];

            std::FILE *in = std::fopen(historyPath, "rb");
            ensure(in != NULL);
            InStream s;
            s.stdfile = true;
            s.reset(in);

            ensure(s.curChar() == 'A');
            ensure(s.reader->getReadChars().empty());
            ensure(s.curChar() == 'A');
            ensure(s.reader->getReadChars().empty());
            ensure(s.nextChar() == 'A');
            ensure(s.reader->getReadChars().empty());

            s.unreadChar('X');
            ensure(s.reader->getReadChars().empty());
            ensure(s.curChar() == 'X');
            ensure(s.reader->getReadChars().empty());
            ensure(s.nextChar() == 'X');
            ensure(s.reader->getReadChars().empty());

            for (size_t bi = 0; bi < sizeof(highBytes) / sizeof(highBytes[0]); bi++) {
                int byte = highBytes[bi];
                ensure(s.reader->nextChar() == byte);
                ensure(s.reader->getReadChars().empty());
                s.unreadChar(char(byte));
                ensure(s.reader->getReadChars().empty());
                ensure(s.reader->curChar() == byte);
                ensure(s.curChar() == char(byte));
                ensure(s.readChar() == char(byte));
                ensure(s.reader->getReadChars().empty());
            }

            ensure(s.reader->getLine() == 1);
            ensure(s.nextChar() == '\n');
            ensure(s.reader->getLine() == 2);
            ensure(s.reader->getReadChars().empty());

            s.reader->setTestCase(0);
            s.reader->setTestCase(1);
            s.reader->setTestCase(__TESTLIB_MAX_TEST_CASE);
            ensure(s.reader->getReadChars().empty());
            ensure_exit(3, [&]() { s.reader->setTestCase(-1); });
            ensure_exit(3, [&]() { s.reader->setTestCase(__TESTLIB_MAX_TEST_CASE + 1); });
            ensure(s.reader->getReadChars().empty());

            ensure(s.readInt() == 42);
            ensure(s.nextChar() == '\n');
            ensure(s.reader->getLine() == 3);
            ensure(s.eof());
            ensure(s.reader->curChar() == EOFC);
            ensure(s.curChar() == char(EOFC));
            ensure(s.reader->nextChar() == EOFC);
            ensure(s.nextChar() == char(EOFC));
            ensure(s.reader->getReadChars().empty());
        }
    }

    // 2. Validator mode must preserve exact readChars / undoReadChars semantics.
    {
        testlibMode = _validator;

        // Scenario A: normal reads, unreadChar replacement, setTestCase, and EOF via nextChar().
        {
            const char raw[] = {'A', 'B', '\n', 'C'};
            std::FILE *out = std::fopen(historyPath, "wb");
            ensure(out != NULL);
            ensure(std::fwrite(raw, 1, sizeof(raw), out) == sizeof(raw));
            std::fclose(out);

            std::FILE *in = std::fopen(historyPath, "rb");
            ensure(in != NULL);
            InStream s;
            s.stdfile = true;
            s.reset(in);

            ensure(s.reader->getReadChars() == vector<int>());
            s.reader->setTestCase(1);
            ensure(s.reader->getReadChars() == vector<int>({257}));
            ensure(s.curChar() == 'A');
            ensure(s.reader->getReadChars() == vector<int>({257}));
            ensure(s.nextChar() == 'A');
            ensure(s.reader->getReadChars() == vector<int>({257, 65}));
            s.unreadChar('X');
            ensure(s.reader->getReadChars() == vector<int>({257}));
            ensure(s.nextChar() == 'X');
            ensure(s.reader->getReadChars() == vector<int>({257, 65}));
            ensure(s.curChar() == 'B');
            ensure(s.reader->getReadChars() == vector<int>({257, 65}));
            s.reader->setTestCase(2);
            ensure(s.reader->getReadChars() == vector<int>({257, 65, 258}));
            ensure(s.nextChar() == 'B');
            ensure(s.reader->getReadChars() == vector<int>({257, 65, 258, 66}));
            ensure(s.reader->getLine() == 1);
            ensure(s.nextChar() == '\n');
            ensure(s.reader->getLine() == 2);
            ensure(s.nextChar() == 'C');
            ensure(s.reader->getReadChars() == vector<int>({257, 65, 258, 66, 10, 67}));
            ensure(s.reader->nextChar() == EOFC);
            ensure(s.nextChar() == char(EOFC));
            ensure(s.reader->getReadChars() == vector<int>({257, 65, 258, 66, 10, 67, -1}));
        }

        // Scenario B1: first reaching EOF via curChar() moves -1 to undoReadChars and sets feof().
        {
            const char raw[] = {'P', 'Q'};
            std::FILE *out = std::fopen(historyPath, "wb");
            ensure(out != NULL);
            ensure(std::fwrite(raw, 1, sizeof(raw), out) == sizeof(raw));
            std::fclose(out);

            std::FILE *in = std::fopen(historyPath, "rb");
            ensure(in != NULL);
            InStream s;
            s.stdfile = true;
            s.reset(in);

            ensure(s.nextChar() == 'P');
            ensure(s.nextChar() == 'Q');
            ensure(s.reader->getReadChars() == vector<int>({80, 81}));
            ensure(s.curChar() == char(EOFC));
            ensure(s.reader->curChar() == EOFC);
            ensure(s.reader->nextChar() == EOFC);
            ensure(s.reader->getReadChars() == vector<int>({80, 81}));
        }

        // Scenario B2: first reaching EOF via eof() calls nextChar() without unreadChar(EOFC), keeping -1 in readChars.
        {
            std::FILE *in = std::fopen(historyPath, "rb");
            ensure(in != NULL);
            InStream s;
            s.stdfile = true;
            s.reset(in);

            ensure(s.nextChar() == 'P');
            ensure(s.nextChar() == 'Q');
            ensure(s.reader->getReadChars() == vector<int>({80, 81}));
            ensure(s.eof());
            ensure(s.reader->getReadChars() == vector<int>({80, 81, -1}));
            ensure(s.reader->nextChar() == EOFC);
            ensure(s.reader->getReadChars() == vector<int>({80, 81, -1}));
        }

        // Scenario C: switching testlibMode while a FileInputStreamReader has buffered lookahead/unread
        // must not desynchronize undoChars and undoReadChars, because recordReadChars is fixed at construction.
        {
            testlibMode = _unknown;
            std::FILE *in = std::fopen(historyPath, "rb");
            ensure(in != NULL);
            InStream s;
            s.stdfile = true;
            s.reset(in);

            ensure(s.curChar() == 'P');
            testlibMode = _validator;
            ensure(s.nextChar() == 'P');
            s.unreadChar('Z');
            ensure(s.nextChar() == 'Z');
            ensure(s.nextChar() == 'Q');
            ensure(s.reader->getReadChars().empty());
        }
        {
            testlibMode = _validator;
            std::FILE *in = std::fopen(historyPath, "rb");
            ensure(in != NULL);
            InStream s;
            s.stdfile = true;
            s.reset(in);

            ensure(s.curChar() == 'P');
            testlibMode = _interactor;
            ensure(s.nextChar() == 'P');
            ensure(s.curChar() == 'Q');
            testlibMode = _validator;
            ensure(s.nextChar() == 'Q');
            ensure(s.reader->getReadChars() == vector<int>({80, 81}));
        }
    }

    // 3. Deterministic pseudo-random test with an independent semantic prefix oracle.
    {
        struct Lcg {
            unsigned s;
            unsigned next() {
                s = s * 1103515245u + 12345u;
                return (s >> 16) & 0x7fff;
            }
        };
        struct Step {
            int op;
            int ret;
            int line;
        };

        const unsigned char raw[] = {
            'a', 0x00, 0x7F, 0x80, 0xFE, 0xFF, '\r', '\n',
            'b', 'c', '\n', 0xFF, '\r', 'd', 0x80, '\n',
            'e', 0x00, 'f', '\r', '\n', 'g', 'h', 0xFE,
            'i', '\n', 'j', 0xFF, 0xFF, 'k', '\r', 'l'
        };
        const std::string bytes(reinterpret_cast<const char *>(raw), sizeof(raw));

        std::FILE *out = std::fopen(historyPath, "wb");
        ensure(out != NULL);
        ensure(std::fwrite(bytes.data(), 1, bytes.size(), out) == bytes.size());
        std::fclose(out);

        const TTestlibMode pairModes[2] = {_validator, _interactor};
        for (unsigned seed = 1; seed <= 500; seed++) {
            std::vector<Step> trace[2];
            std::vector<int> model;

            for (int mi = 0; mi < 2; mi++) {
                testlibMode = pairModes[mi];
                std::FILE *in = std::fopen(historyPath, "rb");
                ensure(in != NULL);
                InStream s;
                s.stdfile = true;
                s.reset(in);

                Lcg g = {seed};
                size_t pos = 0;
                int budget = 0;
                bool eofSeen = false;
                int tc = 0;
                model.clear();

                for (int step = 0; step < 200; step++) {
                    int op = int(g.next() % 6);
                    int ret = 0;
                    if ((op == 2 || op == 3) && eofSeen)
                        op = 0;
                    if (op == 3 && budget == 0)
                        op = 1;

                    switch (op) {
                    case 0:
                        ret = s.reader->curChar();
                        if (ret == EOFC)
                            eofSeen = true;
                        break;
                    case 1:
                    case 2:
                        if (op == 1)
                            ret = s.reader->nextChar();
                        else
                            s.reader->skipChar();
                        if (!eofSeen) {
                            if (pos < bytes.size()) {
                                model.push_back((unsigned char)bytes[pos++]);
                                budget++;
                            } else {
                                model.push_back(-1);
                                eofSeen = true;
                            }
                        }
                        break;
                    case 3:
                        s.reader->unreadChar(int(g.next() % 256));
                        pos--;
                        budget--;
                        model.pop_back();
                        break;
                    case 4:
                        ret = s.reader->eof() ? 1 : 0;
                        if (!eofSeen && pos == bytes.size()) {
                            model.push_back(-1);
                            eofSeen = true;
                        } else if (ret) {
                            eofSeen = true;
                        }
                        break;
                    case 5:
                        tc = tc % 5 + 1;
                        s.reader->setTestCase(tc);
                        model.push_back(256 + tc);
                        // Reset budget so unreadChar is not called right after setTestCase(tc),
                        // which would pop the testcase marker (tc + 256) from readChars.
                        budget = 0;
                        break;
                    }
                    Step st = {op, ret, s.reader->getLine()};
                    trace[mi].push_back(st);
                }

                if (mi == 0)
                    ensure(s.reader->getReadChars() == model);
                else
                    ensure(s.reader->getReadChars().empty());
            }

            ensure(trace[0].size() == trace[1].size());
            for (size_t i = 0; i < trace[0].size(); i++) {
                ensure(trace[0][i].op == trace[1][i].op);
                ensure(trace[0][i].ret == trace[1][i].ret);
                ensure(trace[0][i].line == trace[1][i].line);
            }
        }
    }

    std::remove(historyPath);
}
