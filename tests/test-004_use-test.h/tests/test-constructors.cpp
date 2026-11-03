TEST(constructors) {
    {
        ValidatorBoundsHit h;
        ensure(!h.minHit && !h.maxHit);
        ValidatorBoundsHit hMin(true);
        ensure(hMin.minHit && !hMin.maxHit);
        ValidatorBoundsHit h00(false, false);
        ensure(!h00.minHit && !h00.maxHit);
        ValidatorBoundsHit h01(false, true);
        ensure(!h01.minHit && h01.maxHit);
        ValidatorBoundsHit h10(true, false);
        ensure(h10.minHit && !h10.maxHit);
        ValidatorBoundsHit h11(true, true);
        ensure(h11.minHit && h11.maxHit);
    }
    {
        ensure(exit_exception(0).getExitCode() == 0);
        ensure(exit_exception(3).getExitCode() == 3);
        ensure(exit_exception(7).getExitCode() == 7);
    }
    {
        const char *path = "test-004-readers.tmp";
        std::FILE *out = std::fopen(path, "wb");
        ensure(out != NULL);
        std::fputs("ab\nc", out);
        std::fclose(out);

        std::FILE *in = std::fopen(path, "rb");
        ensure(in != NULL);
        FileInputStreamReader reader(in, "name-1");
        ensure(reader.getName() == "name-1");
        ensure(reader.getLine() == 1);
        ensure(reader.curChar() == 'a');
        ensure(reader.nextChar() == 'a');
        ensure(reader.nextChar() == 'b');
        ensure(reader.nextChar() == '\n');
        ensure(reader.getLine() == 2);
        ensure(reader.nextChar() == 'c');
        ensure(reader.nextChar() == EOFC);
        ensure(reader.eof());
        reader.close();

        in = std::fopen(path, "rb");
        ensure(in != NULL);
        BufferedFileInputStreamReader buffered(in, "name-2");
        ensure(buffered.getName() == "name-2");
        ensure(buffered.getLine() == 1);
        ensure(buffered.curChar() == 'a');
        ensure(buffered.nextChar() == 'a');
        ensure(buffered.nextChar() == 'b');
        ensure(buffered.nextChar() == '\n');
        ensure(buffered.getLine() == 2);
        ensure(buffered.nextChar() == 'c');
        ensure(buffered.nextChar() == EOFC);
        ensure(buffered.eof());
        buffered.close();

        std::remove(path);
    }
}
