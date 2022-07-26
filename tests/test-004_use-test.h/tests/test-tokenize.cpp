TEST(tokenize) {
    ensure(tokenize(" a  b", ' ') == vector<string>({"a", "b"}));
    ensure(tokenize(" best code\t4ever   now!   ", " \t") == vector<string>({"best", "code", "4ever", "now!"}));
    ensure(tokenize("abacabacaba", "ac") == vector<string>({"b", "b", "b"}));
    ensure(tokenize("", ',') == vector<string>());
}
