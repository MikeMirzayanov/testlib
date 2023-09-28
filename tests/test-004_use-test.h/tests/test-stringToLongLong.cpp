TEST(stringToLongLong) {
    ensure(stringToLongLong(inf, "1234567891") == 1234567891LL);
    ensure(stringToLongLong(inf, "-47292722921111") == -47292722921111LL);
    ensure(stringToLongLong(inf, "0") == 0LL);
    ensure(stringToLongLong(inf, "9223372036854775807") == 9223372036854775807LL);
    ensure(stringToLongLong(inf, "-9223372036854775808") == -9223372036854775807LL - 1LL);
    ensure_exit(3, [](){stringToLongLong(inf, "");});
    ensure_exit(3, [](){stringToLongLong(inf, "-");});
    ensure_exit(3, [](){stringToLongLong(inf, "+");});
    ensure_exit(3, [](){stringToLongLong(inf, "00");});
    ensure_exit(3, [](){stringToLongLong(inf, "0123");});
    ensure_exit(3, [](){stringToLongLong(inf, "+123");});
    ensure_exit(3, [](){stringToLongLong(inf, "09223372036854775807");});
    ensure_exit(3, [](){stringToLongLong(inf, "9223372036854775808");});
    ensure_exit(3, [](){stringToLongLong(inf, "-09223372036854775808");});
    ensure_exit(3, [](){stringToLongLong(inf, "-9223372036854775809");});
    ensure_exit(3, [](){stringToLongLong(inf, "1 ");});
    ensure_exit(3, [](){stringToLongLong(inf, " 1");});
    ensure_exit(3, [](){stringToLongLong(inf, "1 2");});
    ensure_exit(3, [](){stringToLongLong(inf, "-0");});
    ensure_exit(3, [](){stringToLongLong(inf, "--0");});
    ensure_exit(3, [](){stringToLongLong(inf, "123-");});
}
