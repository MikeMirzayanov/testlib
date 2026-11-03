TEST(pattern) {
    {
        ensure(!pattern("a").matches(""));
        ensure(!pattern("a").matches("b"));
        ensure(pattern("a").matches("a"));
        ensure(pattern("[^0-9]*").matches(""));
        ensure(pattern("[^0-9]*").matches("wertyuiop(*&^%$#@!~"));
        ensure(pattern("[^0-9]*").matches("?><"));
        ensure(!pattern("[^0-9]*").matches("?>3<"));
        ensure(pattern("id-([ac]|b{2})").matches("id-a"));
        ensure(pattern("id-([ac]|b{2})").matches("id-c"));
        ensure(pattern("id-([ac]|b{2})").matches("id-bb"));
        ensure(!pattern("id-([ac]|b{2})").matches("id-aa"));
        ensure(!pattern("id-([ac]|b{2})").matches("id-cb"));
        ensure(!pattern("id-([ac]|b{2})").matches("id-b"));
        ensure(pattern("[0-9][a-z]{1,3}.[!]{0,2}").matches("4me.!"));
        ensure(pattern("[0-9][a-z]{1,3}.[!]{0,2}").matches("0z."));
        ensure(pattern("[0-9][a-z]{1,3}.[!]{0,2}").matches("9ttt.!!"));
        ensure(pattern("[0-9][a-z]{1,3}.[!]{0,2}").matches("7oe."));
        ensure(!pattern("[0-9][a-z]{1,3}.[!]{0,2}").matches("4me.!!!"));
        ensure(!pattern("[0-9][a-z]{1,3}.[!]{0,2}").matches("rz."));
        ensure(!pattern("[0-9][a-z]{1,3}.[!]{0,2}").matches("9A.!!"));
        ensure(!pattern("[0-9][a-z]{1,3}.[!]{0,2}").matches("7oe"));
        ensure(!pattern("No solution").matches("No solution"));
        ensure(pattern("No\\ solution").matches("No solution"));
        ensure(!pattern("((aa|ba)|(ac|dd))|yes").matches("No solution"));
        ensure(!pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches(""));
        ensure(!pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooxbaydd"));
        ensure(pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooxba"));
        ensure(pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooyac"));
        ensure(pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooydd"));
        ensure(pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooxaa"));
        ensure(pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("yes"));
        ensure(!pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooyes"));
        ensure(!pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("oo"));
        ensure(!pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooyaa"));
        ensure(!pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooxac"));
        ensure(!pattern("oo(x(aa|ba)|y(ac|dd))|yes").matches("ooyddd"));
    }
}

TEST(pattern_src_and_errors) {
    // src() returns the pattern as given; spaces are dropped only for matching.
    ensure(pattern(" [a-c] {2} ").src() == " [a-c] {2} ");
    ensure(pattern(" [a-c] {2} ").matches("ab"));
    ensure(!pattern(" [a-c] {2} ").matches(" ab"));
    ensure(pattern("No solution").src() == "No solution");
    ensure(pattern("No solution").matches("Nosolution"));
    ensure(!pattern("No solution").matches("No solution"));
    ensure(pattern("No\\ solution").src() == "No\\ solution");
    ensure(pattern("No\\ solution").matches("No solution"));
    ensure(!pattern("No\\ solution").matches("Nosolution"));
    // The error message shows the pattern without spaces: "ab)".
    ensure_exit(3, [](){pattern("a b)");});
}
