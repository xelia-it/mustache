////////////////////////////////////////////////////////////////////////////////
///
/// @file       test-basic-rendering.hpp
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
/// @brief      Mustache test suite (basic rendering).
///
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
using std::string;

#include <catch2/catch.hpp>

#include "../../src/mustache-light.hpp"
using mustache::Mustache;

TEST_CASE("Basic rendering") {
    Mustache m("./test/fixtures/");

    SECTION("Render empty template with null JSON") {
        string res = m.render("", "null"_json);
        REQUIRE(res.empty());
        REQUIRE(m.error().empty());
    }

    SECTION("Render empty template with empty string") {
        string res = m.render("", string("{}"));
        REQUIRE(res.empty());
        REQUIRE(m.error().empty());
    }

    SECTION("Render empty template with empty JSON") {
        string res = m.render("", "{}"_json);
        REQUIRE(res.empty());
        REQUIRE(m.error().empty());
    }

    SECTION("Render basic view and context") {
        string html = "<html><body></body></html>";
        string res = m.render(html, string("{}"));
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Render empty view and context (by file)") {
        string res = m.renderFilenames("basic/empty", "basic/empty");
        REQUIRE(res.empty());
        REQUIRE(m.error().empty());
    }

    SECTION("Render basic html with empty context") {
        string html = "<!DOCTYPE html>\n"
                "<html>\n"
                "    <head>\n"
                "        <title>Title</title>\n"
                "    </head>\n"
                "    <body>\n"
                "        <h1>Title</h1>\n"
                "    </body>\n"
                "</html>";
        string res = m.renderFilenames("basic/simple-html", "basic/empty");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Render two equal variables") {
        string html = "VariableVariable";
        string res = m.renderFilenames("basic/two-equal-variables",
                "basic/two-equal-variables");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Render basic html with empty context") {
        string html = "\n"
            "<p>1st Paragraph</p>\n"
            "\n"
            "<p>2nd Paragraph</p>\n";
        string res = m.renderFilenames("basic/comments", "basic/empty");
        std::cout <<m.error() <<std::endl;
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }
}

////////////////////////////////////////////////////////////////////////////////
