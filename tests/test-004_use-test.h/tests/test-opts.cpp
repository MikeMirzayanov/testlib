TEST(opts) {
    suppressEnsureNoUnusedOpts();
    {
        const char* args[] = {"test"};
        prepareOpts(sizeof(args) / sizeof(const char*), (char **)args);
        ensure(!has_opt("a"));
        ensure_exit(3, [](){ opt<int>(1);});
    }
    {
        const char* args[] = {"test", "-a", "-2147483648", "-f1", "0", "-f2", "1", "-f3", "true", "-f4", "false", "--test=me=me", "--d=-17.125", "--ex=3e6"};
        prepareOpts(sizeof(args) / sizeof(const char*), (char **)args);
        ensure(has_opt("a"));
        ensure(has_opt("f1"));
        ensure(has_opt("f2"));
        ensure(has_opt("f3"));
        ensure(has_opt("f4"));
        ensure(has_opt("test"));
        ensure(has_opt("d"));
        ensure(has_opt("ex"));
        ensure(!has_opt("f"));
        ensure(!has_opt("f5"));
        ensure(!has_opt("true"));
        ensure(!has_opt("0"));
        ensure(!has_opt("1"));
        ensure(!has_opt("-a"));
        ensure(!has_opt("--test"));
        ensure(opt<int>("a") == INT_MIN);
        ensure(!opt<bool>("f1"));
        ensure(opt<bool>("f2"));
        ensure(opt<bool>("f3"));
        ensure(!opt<bool>("f4"));
        ensure(opt<string>("test") == "me=me");
        ensure(opt<double>("d") == -17.125);
        ensure(opt<int>("ex") == 3000000);
        ensure(opt<long long>("ex") == 3000000);
        ensure_exit(3, [](){opt<short>("ex");});
        ensure(opt<int>(2) == INT_MIN);
        ensure(opt<string>(13) == "--ex=3e6");
        ensure_exit(3, [](){ opt<string>(-1);});
        ensure("test" == opt<string>(0));
        ensure_exit(3, [](){ opt<bool>(2);});
        ensure_exit(3, [](){ opt<int>(3);});
        ensure_exit(3, [](){ opt<int>(14);});
        ensure_exit(3, [](){ opt<unsigned long long>("a");});
    }
    
    {
        // Testing opt with default value and unused opt.
        // The following function are opts of the generators/gen-array-with-opt.cpp
        // Test is conducted by throwing arguments to this function.
        struct GenArrayOpts {
            int test_count;
            int sum_n, min_n;
            int min_value, max_value, value_bias;
        };
        auto gen_array_with_opt_parse_opt = [](const string& opts) {
            {
                auto parts = split(opts, ' ');
                vector<const char*> opts(parts.size() + 1);
                opts[0] = "./gen-array-with-opt";
                for (size_t i = 0; i < parts.size(); ++i) {
                    opts[i + 1] = parts[i].c_str();
                }
                prepareOpts(int(opts.size()), (char**)opts.data());
            }
            {
                GenArrayOpts res;
                res.test_count = opt<int>("test-count");
                res.sum_n = opt<int>("sum-n");
                res.min_n = opt<int>("min-n", 1);
                
                res.min_value = opt<int>("min-value", 1);
                res.max_value = opt<int>("max-value", 1000 * 1000 * 1000);
                res.value_bias = opt<int>("value-bias", 0);
                
                ensureNoUnusedOpts();
                return res;
            }
        };
        
        // missing test-count
        ensure_exit(3, [&] { gen_array_with_opt_parse_opt(""); });
        // missing sum-n
        ensure_exit(3, [&]{ gen_array_with_opt_parse_opt("-test-count 3"); });
        // min-value, not min-val
        ensure_exit(3, [&]{ gen_array_with_opt_parse_opt("-test-count 3 -sum-n 10 -min-val 1"); } );
        // max-value, not max-val
        ensure_exit(3, [&]{ gen_array_with_opt_parse_opt("-test-count 3 -sum-n 10 -min-value 1 -max-val 20"); } );
        // min-n, not min-length
        ensure_exit(3, [&]{ gen_array_with_opt_parse_opt("-test-count 3 -sum-n 10 -min-length 3 -min-value 10 -max-value 20"); });
        // value-bias, not bias-value
        ensure_exit(3, [&]{ gen_array_with_opt_parse_opt("-test-count 3 -sum-n 10 -min-n 3 -min-value 10 -max-value 20 -bias-value 3"); });
        
        // OK cases
        {
            auto t = gen_array_with_opt_parse_opt("-test-count 3 -sum-n 10");
            ensure(t.test_count == 3);
            ensure(t.sum_n == 10);
            ensure(t.min_n == 1);
            ensure(t.min_value == 1);
            ensure(t.max_value == 1000 * 1000 * 1000);
            ensure(t.value_bias == 0);
        }
        
        {
            auto t = gen_array_with_opt_parse_opt("-test-count 3 -sum-n 10 -min-value 10");
            ensure(t.test_count == 3);
            ensure(t.sum_n == 10);
            ensure(t.min_n == 1);
            ensure(t.min_value == 10);
            ensure(t.max_value == 1000 * 1000 * 1000);
            ensure(t.value_bias == 0);
        }
        
        {
            auto t = gen_array_with_opt_parse_opt("-test-count 5 -sum-n 100 -min-n 3 -min-value 10 -max-value 20 -value-bias 3 ");
            ensure(t.test_count == 5);
            ensure(t.sum_n == 100);
            ensure(t.min_n == 3);
            ensure(t.min_value == 10);
            ensure(t.max_value == 20);
            ensure(t.value_bias == 3);
        }
    }
}

