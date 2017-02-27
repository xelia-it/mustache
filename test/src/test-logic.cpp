////////////////////////////////////////////////////////////////////////////////
///
/// @file       test-logic.hpp
/// @author     Xelia snc <info@xelia.it>
/// @copyright  The code is licensed under the MIT License.
///
///             <http://opensource.org/licenses/MIT>:
///
///             Copyright (c) 2015 Xelia snc
///
///             Permission is hereby granted, free of charge, to any person
///             obtaining a copy of this software and associated documentation
///             files (the "Software"), to deal in the Software without
///             restriction, including without limitation the rights to use,
///             copy, modify, merge, publish, distribute, sublicense, and/or
///             sell copies of the Software, and to permit persons to whom
///             the Software is furnished to do so, subject to the following
///             conditions:
///
///             The above copyright notice and this permission notice shall be
///             included in all copies or substantial portions of the Software.
///
///             THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
///             EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
///             OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
///             NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
///             HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
///             WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
///             ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
///             THE USE OR OTHER DEALINGS IN THE SOFTWARE.
///
/// @brief      Mustache test suite (test logic operators).
///
////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"

#include <string>
#include <mustache-light.hpp>

using std::string;
using mustache::Mustache;

TEST_CASE("Logic operators") {
    Mustache m("./test/fixtures/");

    SECTION("Logic visibility") {
        string html = "<h3></h3>\n"
                "\n"
                "\n"
                "<p>yes is true</p>\n"
                "\n"
                "\n"
                "\n"
                "\n"
                "\n"
                "\n"
                "\n"
                "\n"
                "<p>no is false</p>\n";
        string res = m.renderFilenames("logic/logic", "logic/logic");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Logic visibility (negated)") {
        string html = "<h3></h3>\n"
                "\n"
                "\n"
                "\n"
                "\n"
                "<p>yes is false</p>\n"
                "\n"
                "\n"
                "\n"
                "<p>no is true</p>\n"
                "\n"
                "\n";
        string res = m.renderFilenames("logic/logic", "logic/logic-negated");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Logic nested") {
        string html = "YES\n"
                "YES\n";
        string res = m.renderFilenames("logic/nested", "logic/nested");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Logic nested (negated)") {
        string html = "NO\n"
                "NO\n";
        string res = m.renderFilenames("logic/nested", "logic/nested-negated");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }
}

////////////////////////////////////////////////////////////////////////////////
