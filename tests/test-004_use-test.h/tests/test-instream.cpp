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
