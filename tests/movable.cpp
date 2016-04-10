#include <LuaContext.hpp>
#include <gtest/gtest.h>

TEST(Movable, PreserveValues) {
    LuaContext context1;
    context1.writeVariable("a", "hello");

    LuaContext context2 = std::move(context1);
    EXPECT_EQ("hello", context2.readVariable<std::string>("a"));
}

TEST(Movable, PreserveRegisteredFunctions) {
    struct Foo {
        int foo() { return 3; }
    };

    LuaContext context1;
    context1.registerFunction("foo", &Foo::foo);
    context1.writeVariable("a", Foo{});

    LuaContext context2 = std::move(context1);
    EXPECT_EQ(3, context2.executeCode<int>("return a:foo()"));
}

TEST(Movable, PreserveReadFunctions) {
    LuaContext context1;
    context1.executeCode("f = function(i) return i + 1; end");
    auto f = context1.readVariable<std::function<int (int)>>("f");

    LuaContext context2 = std::move(context1);
    EXPECT_EQ(3, f(2));

    f = {};
}
