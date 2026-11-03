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
