TEST(instream_skip_bom) {
    {
        InStream s(inf, "\xEF\xBB\xBF" "content");
        ensure(s.readWord() == "content");
        ensure(s.eof());
    }
}
