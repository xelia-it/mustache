////////////////////////////////////////////////////////////////////////////////
///
/// @file       test-partials.hpp
/// @author     Xelia snc <info@xelia.it>
/// @copyright  The code is licensed under the MIT License.
///
///             <http://opensource.org/licenses/MIT>:
///
///             Copyright (c) Xelia snc
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
/// @brief      Mustache test suite (test partial functionality).
///
////////////////////////////////////////////////////////////////////////////////

#include <string>
using std::string;

#include "../../catch.hpp"

#include "../../src/mustache-light.hpp"
using mustache::Mustache;

TEST_CASE("Partials") {
    Mustache m("./test/fixtures/");

    SECTION("Simple partial") {
        string html = "<!DOCTYPE html>";
        string res = m.renderFilenames("partials/simple", "partials/simple");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Nested partials") {
        string expected = "<!DOCTYPE html>\n"
                "<html>\n"
                "    <head>\n"
                "        <title>My Title</title>\n"
                "    </head>\n"
                "    <body>\n"
                "        <b>First level</b>\n"
                "        <b>Second level</b>\n"
                "        <p>\n"
                "        variable-001\n"
                "        variable-002\n"
                "        variable-003\n"
                "        variable-004\n"
                "        variable-005\n"
                "        variable-006\n"
                "        variable-007\n"
                "        variable-008\n"
                "        variable-009\n"
                "        variable-010\n"
                "        </p>\n"
                "        <b>Third level</b>\n"
                "        <p>Warning!</p>\n"
                "\n"
                "    </body>\n"
                "</html>\n"
                "<p>Created by Mustache - End of page My Title</p>\n";

        string res = m.renderFilenames("partials/nested", "partials/nested");
        REQUIRE(res == expected);
        REQUIRE(m.error().empty());
    }

    SECTION("Partials with variables") {
        string expected = "1st\n"
                "[]\n"
                "2nd\n"
                "3rd\n";
        string res = m.renderFilenames("partials/with-variables",
                "partials/with-variables");
        REQUIRE(res == expected);
        REQUIRE(m.error().empty());
    }

    SECTION("Multiple partials with variables") {
        string expected = "<h3>User List</h3>\n"
                "\n"
                "\n"
                "<div>\n"
                "    <ul>\n"
                "        <li>Name: Mario</li>\n"
                "        <li>Surname: Rossi</li>\n"
                "    </ul>\n"
                "</div>\n"
                "\n"
                "\n"
                "<div>\n"
                "    <ul>\n"
                "        <li>Name: John</li>\n"
                "        <li>Surname: Smith</li>\n"
                "    </ul>\n"
                "</div>\n"
                "\n"
                "\n"
                "<div>\n"
                "    <ul>\n"
                "        <li>Name: Giulio</li>\n"
                "        <li>Surname: Bianchi</li>\n"
                "    </ul>\n"
                "</div>\n"
                "\n"
                "\n"
                "<div>\n"
                "    <ul>\n"
                "        <li>Name: Valerio</li>\n"
                "        <li>Surname: Verdi</li>\n"
                "    </ul>\n"
                "</div>\n";
        string res = m.renderFilenames("partials/multiple-partials-with-variables",
                "partials/multiple-partials-with-variables");
        REQUIRE(res == expected);
        REQUIRE(m.error().empty());
    }

    SECTION("Partials inside hidden block") {
        string expected = "visible\n"
                "\n"
                "\n"
                "visible\n"
                "\n";
        string res = m.renderFilenames("partials/partial-inside-hidden-block",
                "partials/partial-inside-hidden-block");
        REQUIRE(res == expected);
        REQUIRE(m.error().empty());
    }
}

////////////////////////////////////////////////////////////////////////////////
