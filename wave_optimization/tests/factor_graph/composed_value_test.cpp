#include "wave/wave_test.hpp"
#include "wave/optimization/factor_graph/FactorValue.hpp"

namespace wave {
struct ComposedValueTest : public ::testing::Test {
    // Define a sample composed value (use aliases for sized FactorValue)
    template <typename T, typename O = void>
    struct Composed
      : ComposedValue<T, O, FactorValue1, FactorValue2, FactorValue3> {
        using Base =
          ComposedValue<T, O, FactorValue1, FactorValue2, FactorValue3>;
        using Base::Base;
        Composed() = default;
        Composed(const Base &base) : Base{base} {}
        FactorValue<T, O, 1> &b0 = this->template block<0>();
        FactorValue<T, O, 2> &b1 = this->template block<1>();
        FactorValue<T, O, 3> &b2 = this->template block<2>();
    };
};


TEST_F(ComposedValueTest, construct) {
    Composed<double>{};
}

TEST_F(ComposedValueTest, constructFromLists) {
    auto c = Composed<double>{1.1, {2.2, 3.3}, {4.4, 5.5, 6.6}};
    EXPECT_EQ(1.1, c.b0.value());
    EXPECT_EQ(Vec2(2.2, 3.3), c.b1);
    EXPECT_EQ(Vec3(4.4, 5.5, 6.6), c.b2);
}

TEST_F(ComposedValueTest, getData) {
    auto c = Composed<double>{1.1, {2.2, 3.3}, {4.4, 5.5, 6.6}};
    auto res = c.blockData();
    auto expected =
      std::vector<double *>{c.b0.data(), c.b1.data(), c.b2.data()};
    EXPECT_EQ(expected, res);
}

TEST_F(ComposedValueTest, constructMapped) {
    auto a0 = 1.1;
    auto a1 = Vec2{2.2, 3.3};
    auto a2 = Vec3{4.4, 5.5, 6.6};

    auto c =
      Composed<double, FactorValueOptions::Map>{&a0, a1.data(), a2.data()};
    EXPECT_EQ(a0, c.b0.value());
    EXPECT_EQ(a1, c.b1);
    EXPECT_EQ(a2, c.b2);
    EXPECT_EQ(&a0, c.b0.data());
    EXPECT_EQ(a1.data(), c.b1.data());
    EXPECT_EQ(a2.data(), c.b2.data());

    static_assert(
      std::is_same<FactorValue<double, FactorValueOptions::Map, 1> &,
                   decltype(c.b0)>::value,
      "");
}

TEST_F(ComposedValueTest, subtractCompound) {
    auto a0 = 1.1;
    auto a1 = Vec2{2.2, 3.3};
    auto a2 = Vec3{4.4, 5.5, 6.6};
    auto b0 = 0.9;
    auto b1 = Vec2{0.8, 0.7};
    auto b2 = Vec3{0.6, 0.5, 0.4};

    auto a = Composed<double>{a0, a1, a2};
    const auto b = Composed<double>{b0, b1, b2};
    a -= b;

    EXPECT_DOUBLE_EQ(a0 - b0, a.b0[0]);
    EXPECT_EQ(a1 - b1, a.b1);
    EXPECT_EQ(a2 - b2, a.b2);
}

TEST_F(ComposedValueTest, subtract) {
    auto a0 = 1.1;
    auto a1 = Vec2{2.2, 3.3};
    auto a2 = Vec3{4.4, 5.5, 6.6};
    auto b0 = 0.9;
    auto b1 = Vec2{0.8, 0.7};
    auto b2 = Vec3{0.6, 0.5, 0.4};

    const auto a = Composed<double>{a0, a1, a2};
    const auto b = Composed<double>{b0, b1, b2};
    Composed<double> c = a - b;

    EXPECT_DOUBLE_EQ(a0 - b0, c.b0[0]);
    EXPECT_EQ(a1 - b1, c.b1);
    EXPECT_EQ(a2 - b2, c.b2);
}

}  // namespace wave