TEST(stringToUnsignedLongLong) {
    ensure(stringToUnsignedLongLong(inf, "123") == 123ULL);
    ensure(stringToUnsignedLongLong(inf, "0") == 0ULL);
    ensure(stringToUnsignedLongLong(inf, "7") == 7ULL);
    ensure(stringToUnsignedLongLong(inf, "18446744073709551615") == 18446744073709551615ULL);
    ensure(stringToUnsignedLongLong(inf, "9876543216540001000") == 9876543216540001000ULL);

    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "-");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "-1");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "+");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "00");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "0123");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "+123");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "18446744073709551616");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "18446744073709551617");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "36893488147419103232");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "1 ");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, " 1");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "1 2");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "-0");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "--0");});
    ensure_exit(3, [](){stringToUnsignedLongLong(inf, "-00");});
}
