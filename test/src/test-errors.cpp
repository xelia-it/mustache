////////////////////////////////////////////////////////////////////////////////
///
/// @file       test-errors.cpp
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
/// @brief      Mustache test suite (test error conditions).
///
////////////////////////////////////////////////////////////////////////////////

#include <string>
using std::string;

#include <catch2/catch.hpp>

#include "../../src/mustache-light.hpp"
using mustache::Mustache;

TEST_CASE("Rendering with errors") {
    Mustache m("./test/fixtures/");

    SECTION("Error in parenthesis") {
        string html = "<!DOCTYPE html>\n"
                "<html>\n"
                "    <head>\n"
                "        <title>Page with error</title>\n"
                "    </head>\n"
                "    <body>\n"
                "        <h1>A basic title";
        string res = m.renderFilenames("errors/parenthesis", "errors/errors");
        REQUIRE(res == html);
        REQUIRE(m.error() == "Missing }}");
    }

    SECTION("Error in partial literal") {
        string html = "<!DOCTYPE html>\n"
            "<html>\n"
            "    <head>\n"
            "        <title>Page with partial literal error</title>\n"
            "    </head>\n"
            "    <body>\n"
            "        <h1>A basic title</h1>\n"
            "        ";
        string res = m.renderFilenames("errors/partial-literal", "errors/errors");
        REQUIRE(res == html);
        REQUIRE(m.error() == "Substitution string 'Name not properly closed");
    }

    SECTION("Error in partial separator") {
        string html = "<!DOCTYPE html>\n"
            "<html>\n"
            "    <head>\n"
            "        <title>Page with partial separator error</title>\n"
            "    </head>\n"
            "    <body>\n"
            "        <h1>A basic title</h1>\n"
            "        ";
        string res = m.renderFilenames("errors/partial-separator", "errors/errors");
        REQUIRE(res == html);
        REQUIRE(m.error() == "Bad substitution string: missing separator in Name='John' Surname='Doe'");
    }

    SECTION("Error section not closed") {
        string html = "<!DOCTYPE html>\n"
            "<html>\n"
            "    <head>\n"
            "        <title>Page with section error</title>\n"
            "    </head>\n"
            "    <body>\n"
            "        <h1>A basic title</h1>\n"
            "        ";
        string res = m.renderFilenames("errors/section-not-closed", "errors/errors");
        REQUIRE(res == html);
        REQUIRE(m.error() == "Missing {{/");
    }
}

////////////////////////////////////////////////////////////////////////////////
