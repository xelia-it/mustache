
#define CATCH_CONFIG_MAIN
#include "./catch.hpp"

#include <iostream>
#include <string>
#include <mustache-light.hpp>

using std::string;
using mustache::Mustache;
using mustache::RenderException;

TEST_CASE("Library initialization") {
    REQUIRE_NOTHROW(Mustache m("./test/fixtures/"));
}

TEST_CASE("Basic rendering") {
    Mustache m("./test/fixtures/");

    SECTION("Render default view and string context") {
        string res = m.render("", string("{}"));
        REQUIRE(res.empty());
        REQUIRE(m.error().empty());
    }

    SECTION("Render default view and JSON context") {
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

    SECTION("Render basic html") {
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
}

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
}

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

TEST_CASE("Sections") {
    Mustache m("./test/fixtures/");

    SECTION("Section with data") {
        string html = "<div>\n"
                "<p></p>\n"
                "\n"
                "<p>Data 3</p>\n"
                "</div>";
        string res = m.renderFilenames("sections/sections-with-data",
                "sections/sections-with-data");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Section with list") {
        string html = "<ul>\n"
                "<li>someone@somewhere.com</li>"
                "<li>another.one@somewhere.com</li>\n"
                "</ul>";
        string res = m.renderFilenames("sections/list", "sections/list");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }

    SECTION("Section with list and indexes") {
        string html = "<ul>\n"
                "<li class=\"active\">0 - someone@somewhere.com</li>"
                "<li>1 - another.one@somewhere.com</li>"
                "<li>2 - a.third.person@somewhere.com</li>\n"
                "</ul>";
        string res = m.renderFilenames("sections/list-with-indexes", "sections/list-with-indexes");
        REQUIRE(res == html);
        REQUIRE(m.error().empty());
    }
}

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
                "[2nd]\n"
                "2nd\n"
                "3rd";
        string res = m.renderFilenames("partials/with-variables",
                "partials/with-variables");
        REQUIRE(res == expected);
        REQUIRE(m.error().empty());
    }
}
