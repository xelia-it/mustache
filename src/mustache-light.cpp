////////////////////////////////////////////////////////////////////////////////
///
/// @file       mustache-light.hpp
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
/// @brief      Mustache template parser.
///
////////////////////////////////////////////////////////////////////////////////

#include "./mustache-light.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <codecvt>

// Used for readFile
#include <fstream>
#include <string>
#include <sstream>

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

using std::vector;
using std::string;
using std::map;

namespace mustache {

const string Mustache::VALID_CHARS_FOR_ID = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_@[]";
const string Mustache::VALID_CHARS_FOR_PARTIALS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_/|=[]().,;!?\"' \n\r°&";

const string Mustache::TOKEN_START_VARIABLE = "{{";
const string Mustache::TOKEN_START_VARIABLE_UNESCAPED = "{{{";
const string Mustache::TOKEN_START_COMMENT = "{{!";
const string Mustache::TOKEN_START_BEGIN_SECTION = "{{#";
const string Mustache::TOKEN_START_END_SECTION = "{{/";
const string Mustache::TOKEN_START_IF = "{{=";
const string Mustache::TOKEN_START_EXISTS_TEST = "{{0";
const string Mustache::TOKEN_START_UNLESS = "{{^";
const string Mustache::TOKEN_START_PARTIAL = "{{>";
const string Mustache::TOKEN_START_TEMPLATE = "{{<";
const string Mustache::TOKEN_END = "}}";
const string Mustache::TOKEN_END_UNESCAPED = "}}}";

const string Mustache::DEFAULT_PARTIAL_EXTENSION = "mustache";

#ifdef DEBUG
#    define LOG_START(x) cout << (x)
#    define LOG(x) cout << (x)
#    define LOG_END(x) cout << (x) << endl
#else
#    define LOG_START(x)
#    define LOG(x)
#    define LOG_END(x)
#endif

#define IS_TOKEN(token) \
        (tokens_.at(currentToken_) == (token))

#define IS_TOKEN_EMPTY() \
        (currentToken_ == tokens_.size())

#define CHECK_TOKEN_NOT_EMPTY() \
        if (currentToken_ == tokens_.size()) { \
                error("Unexpected end of file."); \
                return; \
        }

#define CONSUME_TOKEN() \
        ++ currentToken_

#define CHECK_TOKEN_IS(token) \
        if ((currentToken_ == tokens_.size()) || (tokens_.at(currentToken_) != (token))) { \
                error("Missing " + (token)); \
                return; \
        }


#define CHECK_TOKEN_IS_NOT(token) \
        if ((currentToken_ == tokens_.size()) && (tokens_.at(currentToken_) == (token))) { \
                error("Unexpected " + (token)); \
                return; \
        }

// Ensure token is (txt).
// We need to check all known tokens because (txt) is free text.
#define CHECK_TOKEN_IS_TEXT() \
        CHECK_TOKEN_NOT_EMPTY(); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_VARIABLE); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_COMMENT); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_BEGIN_SECTION); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_END_SECTION); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_IF); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_UNLESS); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_EXISTS_TEST); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_PARTIAL); \
        CHECK_TOKEN_IS_NOT(TOKEN_START_TEMPLATE); \
        CHECK_TOKEN_IS_NOT(TOKEN_END)

Mustache::Mustache(const string& basePath) :
        basePath_(basePath), partialExtension_(DEFAULT_PARTIAL_EXTENSION),
        view_(""), context_("{}"),
        currentListCounter_(0), visible_(true) {
}

Mustache::Mustache(const string& basePath, const string& partialExtension) :
        basePath_(basePath), partialExtension_(partialExtension),
        view_(""), context_("{}"),
        currentListCounter_(0), visible_(true) {
}

Mustache::~Mustache() {
        data_.clear();
        context_.clear();
        tokens_.clear();
        rendered_.clear();
}

string Mustache::render(const string& view, const string& context) {
        view_ = view;
        context_ = context;
        try {
                data_ = json::parse(context_);
        } catch (const std::runtime_error& err) {
                rendered_.clear();
                error_ = err.what();
                return rendered_;
        } catch (const std::invalid_argument& err) {
                rendered_.clear();
                error_ = err.what();
                return rendered_;
        }

        return render();
}

string Mustache::render(const string& view, const json& context) {
        view_ = view;
        data_ = context;

        return render();
}

string Mustache::error() const {
        return error_;
}

string Mustache::renderFilenames(const string& viewFileName, const string& contextFileName) {
        const string& view = fileRead(viewFileName);
        const string& context = fileRead(contextFileName, "json");

        return render(view, context);
}



string Mustache::fileRead(const string& fileName, const string& fileExtension) {
        const string realFileName = basePath_ + fileName + "." + fileExtension;
        std::ifstream in(realFileName.c_str(), std::ios::in | std::ios::binary);
        if (in) {
                string contents;
                in.seekg(0, std::ios::end);
                contents.resize(in.tellg());
                in.seekg(0, std::ios::beg);
                in.read(&contents[0], contents.size());
                in.close();
                return (contents);
        }
        error("Cannot open file: " + realFileName);
}

string Mustache::fileRead(const string& fileName) {
        return fileRead(fileName, partialExtension_);
}

string Mustache::render() {
        error_.clear();

        // Tokenize view
#ifdef DEBUG
        LOG_END("BEFORE TOKEN");
        dumpTokens();
#endif
        tokens_ = tokenize(view_);

#ifdef DEBUG
        LOG_END("AFTER TOKEN");
        dumpTokens();
#endif
        currentToken_ = 0;

        // Reset stack: start from a stack containing the whole json
        while (!stack_.empty()) {
                stack_.pop();
        }
        stack_.push(data_);
        visible_ = true;
        rendered_.clear();

        LOG_END("------------------------------------------------------");
        LOG_END("Render:");
        try {
                produceMessage();
        } catch (const RenderException& err) {
                error_ = err.what();
                return rendered_;
        }
        LOG_END("------------------------------------------------------");
        LOG_END("Output:");
        LOG_END(rendered_);
        return rendered_;
}

Mustache::Tokens Mustache::tokenize(const string& view) {
        std::string::size_type start = 0;
        std::string::size_type prev = 0;
        std::string::size_type pos;
        Tokens tokens;

        while ((pos = view.find_first_of("{}", prev)) != std::string::npos) {
                if (view[pos] == '{') {
                        if (view[pos + 1] == '{') {
                                // Insert into tokens text encountered until "{{"
                                tokens.push_back(view.substr(start, pos - start));
                                if (view[pos + 2] == '#') {
                                        tokens.push_back(TOKEN_START_BEGIN_SECTION);
                                        prev = pos + 3;
                                } else if (view[pos + 2] == '=') {
                                        tokens.push_back(TOKEN_START_IF);
                                        prev = pos + 3;
                                } else if (view[pos + 2] == '^') {
                                        tokens.push_back(TOKEN_START_UNLESS);
                                        prev = pos + 3;
                                } else if (view[pos + 2] == '0') {
                                        tokens.push_back(TOKEN_START_EXISTS_TEST);
                                        prev = pos + 3;
                                } else if (view[pos + 2] == '/') {
                                        tokens.push_back(TOKEN_START_END_SECTION);
                                        prev = pos + 3;
                                } else if (view[pos + 2] == '>') {
                                        tokens.push_back(TOKEN_START_PARTIAL);
                                        prev = pos + 3;
                                } else if (view[pos + 2] == '<') {
                                        tokens.push_back(TOKEN_START_TEMPLATE);
                                        prev = pos + 3;
                                } else if (view[pos + 2] == '!') {
                                        tokens.push_back(TOKEN_START_COMMENT);
                                        prev = pos + 3;
                                } else {
                                        if (view[pos + 2] == '{') {
                                                tokens.push_back(TOKEN_START_VARIABLE_UNESCAPED);
                                                prev = pos + 3;
                                        } else {
                                                tokens.push_back(TOKEN_START_VARIABLE);
                                                prev = pos + 2;
                                        }
                                }
                                start = prev;
                        } else {
                                // Single open {: continue..
                                prev = pos + 1;
                        }
                } else {
                        if (view[pos + 1] == '}') {
                                // Trim string inside parenthesis.
                                // Eg: "{{ some }}" => " some " => "some"
                                string token = view.substr(start, pos - start);
                                token = trim(token);
                                // Now save both token and closed parenthesis "}}"
                                tokens.push_back(token);
                                if (view[pos + 2] == '}') {
                                        tokens.push_back(TOKEN_END_UNESCAPED);
                                        prev = pos + 3;
                                } else {
                                        tokens.push_back(TOKEN_END);
                                        prev = pos + 2;
                                }
                                start = prev;
                        } else {
                                // Single close }: continue..
                                prev = pos + 1;
                        }
                }
        }
        // Save last part of the file (if any) as free text
        tokens.push_back(view.substr(start, pos - start));

        return tokens;
}

void Mustache::produceMessage() {
        LOG_START("");
        LOG_END("MESSAGE := ");
        LOG_START("");

        if (IS_TOKEN_EMPTY()) {
                LOG_END("  (empty)");

                return;
        }
        if (IS_TOKEN(TOKEN_START_VARIABLE)) {
                LOG_END("  VARIABLE");
                CONSUME_TOKEN();
                produceVariable();
                CONSUME_TOKEN();
                produceMessage();

                return;
        }
        if (IS_TOKEN(TOKEN_START_VARIABLE_UNESCAPED)) {
                LOG_END("  VARIABLE UNESCAPED");
                CONSUME_TOKEN();
                produceVariableUnescaped();
                CONSUME_TOKEN();
                produceMessage();

                return;
        }
        if (IS_TOKEN(TOKEN_START_COMMENT)) {
                LOG_END("  COMMENT");
                CONSUME_TOKEN();
                produceComment();
                CONSUME_TOKEN();
                produceMessage();

                return;
        }
        if (IS_TOKEN(TOKEN_START_BEGIN_SECTION) || IS_TOKEN(TOKEN_START_IF) ||
            IS_TOKEN(TOKEN_START_UNLESS) || IS_TOKEN(TOKEN_START_EXISTS_TEST)) {
                LOG_END("  SECTION");
                CONSUME_TOKEN();
                produceSection();
                CONSUME_TOKEN();
                produceMessage();
                return;
        }
        if (IS_TOKEN(TOKEN_START_END_SECTION)) {
                //        if (stack_.size() == 1) {
                //            error("Unexpected end of block '" + TOKEN_START_END_SECTION + "'");
                //        }
                // If we are in a block do not throw error.. simply return
                return;
        }
        if (IS_TOKEN(TOKEN_START_PARTIAL) || IS_TOKEN(TOKEN_START_TEMPLATE)) {
                LOG_END("  PARTIAL");
                CONSUME_TOKEN();
                producePartial();
                produceMessage();
                return;
        }
        if (IS_TOKEN(TOKEN_END)) {
                error("Unexpected end of variable '" + TOKEN_END + "'");
                return;
        }

        LOG_END("  (text)");
        if (visible_) {
                rendered_.append(tokens_.at(currentToken_));
        }
        CONSUME_TOKEN();
        produceMessage();
}

void Mustache::produceVariable()
{
        LOG("VARIABLE := ");
        LOG_START(TOKEN_START_VARIABLE);

        // Token must be (txt)
        CHECK_TOKEN_IS_TEXT();

        printVariable(true);

        CONSUME_TOKEN();
        CHECK_TOKEN_NOT_EMPTY();
        CHECK_TOKEN_IS(TOKEN_END);
        LOG("  ");
        LOG_END(TOKEN_END);
}

void Mustache::produceVariableUnescaped()
{
        LOG("VARIABLE_UNESCAPED := ");
        LOG_START(TOKEN_START_VARIABLE_UNESCAPED);

        // Token must be (txt)
        CHECK_TOKEN_IS_TEXT();

        printVariable(false);

        CONSUME_TOKEN();
        CHECK_TOKEN_NOT_EMPTY();
        CHECK_TOKEN_IS(TOKEN_END_UNESCAPED);
        LOG("  ");
        LOG_END(TOKEN_END_UNESCAPED);
}

void Mustache::printVariable(bool escape_html)
{
    const string& variable_name = tokens_.at(currentToken_);
    ensureValidIdentifier(variable_name);
    LOG(" ");
    LOG(variable_name);
    if (visible_) {
        json variable;
        try {
            variable = searchVariableInContext(variable_name);
        } catch(const std::out_of_range& ex) {
            variable = nullptr;
        } catch(const std::invalid_argument& ex) {
            variable = nullptr;
        }

        if (variable.is_primitive()) {
            if (variable.is_null()) {
                // OK
            } else if (variable.is_boolean()) {
                if (variable.get<bool>()) {
                    rendered_.append("true");
                }
            } else if (variable.is_string()) {
                string to_be_appended = variable.get<string>();
                if (escape_html) {
                    htmlEscape(to_be_appended);
                }
                rendered_.append(to_be_appended);
            } else {
                rendered_.append(variable.dump());
            }
        }
    }
    // else skip render invisible parts
}

void Mustache::produceComment() {
        LOG_START("COMMENT := ");
        LOG_END(TOKEN_START_VARIABLE);

        // Token must be (txt)
        LOG_END("token not empty  ");
        CHECK_TOKEN_IS_TEXT();

        LOG_END("consume token  ");
        CONSUME_TOKEN();
        LOG_END("check token not empty  ");
        CHECK_TOKEN_NOT_EMPTY();
        LOG_END("check token is  ");
        CHECK_TOKEN_IS(TOKEN_END);

        LOG("  ");
        LOG_END(TOKEN_END);
}

void Mustache::produceSection() {
        bool useSection = (tokens_.at(currentToken_ - 1) == TOKEN_START_BEGIN_SECTION);
        bool useUnless = (tokens_.at(currentToken_ - 1) == TOKEN_START_UNLESS);
        bool useExistsTest = (tokens_.at(currentToken_ - 1) == TOKEN_START_EXISTS_TEST);

        LOG_START("");
        LOG_END("SECTION := ");
#ifdef DEBUG
        if (useSection) {
                LOG_START(TOKEN_START_BEGIN_SECTION);
        } else if (useUnless) {
                LOG_START(TOKEN_START_UNLESS);
        } else if (useExistsTest) {
                LOG_START(TOKEN_START_EXISTS_TEST);
        } else {
                LOG_START(TOKEN_START_IF);
        }
#endif

        // Token must be (txt)
        CHECK_TOKEN_IS_TEXT();

        // Save var name
        string variableName = tokens_.at(currentToken_);
        ensureValidIdentifier(variableName);
        LOG(" ");
        LOG(variableName);

        json variable;
        bool variable_exists;
        try {
            variable = searchVariableInContext(variableName);
            variable_exists = true;
        } catch(const std::out_of_range& ex) {
            variable = nullptr;
            variable_exists = false;
        } catch(const std::invalid_argument& ex) {
            variable = nullptr;
            variable_exists = false;
        }

        // Is variable malformed
        bool isCorrectType = variable.is_null() || variable.is_boolean() ||
                             variable.is_string() || variable.is_array() ||
                             variable.is_object() || variable.is_number();
        if (!isCorrectType) {
                error("Variable '" + variableName + "' is malformed");
                return;
        }

        CONSUME_TOKEN();
        CHECK_TOKEN_NOT_EMPTY();
        CHECK_TOKEN_IS(TOKEN_END);
        LOG(" ");
        LOG_END(TOKEN_END);

        CONSUME_TOKEN();
        // The logic of section block {{# }}
        // - [ a, b, c, .. ] ==> cycle
        // - [], {}, "", false, null ==> hide
        // - { something }, "something", true ==> view
        // Section always changes context. EG:
        //
        // context = {
        //   "var1": { "one": 1, "two": 2 },
        //   "var2": "value2"
        // }
        //
        // template =  {{# var1 }}{{ one }}{{/ var1 }}
        //
        // output = 1
        //
        if (useSection && variable.is_array() && variable.size() > 0) {
                const TokenIndex savedPosition = currentToken_;
                for (json::iterator it = variable.begin(); it != variable.end(); ++it) {
                        LOG_END(variable);
                        currentToken_ = savedPosition;
                        currentListCounter_ = std::distance(variable.begin(), it);
                        stack_.push(*it);
                        produceMessage();
                        stack_.pop();
                }
                // Reset to 0 after the main cycle
                currentListCounter_ = 0;
        } else {
                // The hide variable is used for {{= }} and {{# }} logic
                bool hide =
                        (variable.is_array() && variable.size() == 0) ||
                        (variable.is_object() && variable.size() == 0) ||
                        (variable.is_string() && variable.get<string>().size() == 0) ||
                        (variable.is_boolean() && !variable.get<bool>()) ||
                        (variable.is_number() && variable.get<int>() == 0) ||
                        (variable.is_null());

                if (useExistsTest) {
                        // The exist test {{0 }} uses a different logic:
                        // If the key do not exists the section is not shown.
                        hide = !variable_exists;
                } else if (useUnless) {
                        // The inverted section {{^ }} uses inverted logic
                        hide = !hide;
                }
                bool oldVisible = visible_;

                // Should the next message be visible?
                visible_ = visible_ && !hide;

                // The only difference from {{# }} and {{= }} {{^ }} {{? }}
                // is the fact that tag {{# }} changes context.
                if (useSection) {
                        stack_.push(variable);
                }
                produceMessage();
                if (useSection) {
                        stack_.pop();
                }

                // Retrieve the old visibility state
                visible_ = oldVisible;
        }

        CHECK_TOKEN_IS(TOKEN_START_END_SECTION);
        LOG(" ");
        LOG(TOKEN_START_END_SECTION);

        CONSUME_TOKEN();
        CHECK_TOKEN_NOT_EMPTY();
        const string& variableNameEnd = tokens_.at(currentToken_);
        LOG(" ");
        LOG(variableNameEnd);
        ensureValidIdentifier(variableName);
        if (variableNameEnd != variableName) {
                error("Expected '" + variableName + "' in closing block (found '" +
                      variableNameEnd + "')");
                return;
        }

        CONSUME_TOKEN();
        CHECK_TOKEN_NOT_EMPTY();
        CHECK_TOKEN_IS(TOKEN_END);
        LOG("  ");
        LOG_END(TOKEN_END);
}

void Mustache::producePartial() {
        bool useTemplate = (tokens_.at(currentToken_ - 1) == TOKEN_START_TEMPLATE);
        LOG("PARTIAL := ");
        LOG_END("");
        LOG_START("  ");

        // Token must be (txt)
        CHECK_TOKEN_IS_TEXT();

        // Variable completePartialToken should be something like:
        //   paragraph.mustache|title=SampleTitle|text=SampleText
        // The first part is the partial file name.
        const string& partialToken = tokens_.at(currentToken_);
        LOG(" ");
        LOG(partialToken);
        LOG(" ");
        ensureValidIdentifier(partialToken, VALID_CHARS_FOR_PARTIALS);

        CONSUME_TOKEN();
        CHECK_TOKEN_NOT_EMPTY();
        CHECK_TOKEN_IS(TOKEN_END);
        LOG(" ");
        LOG_END(TOKEN_END);

        // Split token using partial variable separator.
        Tokens splitted = split(partialToken, '|');
        for (size_t i = 0; i < splitted.size(); i++) {
                splitted.at(i) = trim(splitted.at(i));
        }

        // Tokenize
        string fileToRead;
        if (useTemplate) {
                LOG_END("  Use template");
                fileToRead = getTemplateNameFromContext(splitted.at(0));
        } else {
                LOG_END("  Use normal partial");
                fileToRead = splitted.at(0);
        }
        LOG("  File to read: ");
        LOG_END(fileToRead);
        string viewFromPartial = fileRead(fileToRead);
        Tokens newTokens = tokenize(viewFromPartial);

        // TODO: can we do better?
        if (splitted.size() > 1) {
                partialSubstitute(splitted, newTokens);
        }

        // Redundant but useful
        CHECK_TOKEN_IS_NOT(TOKEN_START_PARTIAL);

        // TODO: Can we avoid iterators?
        currentToken_ = (currentToken_ - 2);
        Tokens::iterator current = std::next( tokens_.begin(), currentToken_);
        tokens_.erase(current, current + 3);
        tokens_.insert(current, newTokens.begin(), newTokens.end());
    #ifdef DEBUG
        LOG_END("TOKEN AFTER SUBSTITUTION");
        dumpTokens();
    #endif
}

void Mustache::partialSubstitute(const Tokens partialParams, Tokens& newTokens) {
        Variables partialVariables;
        LOG_END("===============================================");

        for (std::vector<string>::size_type i = 1; i != partialParams.size(); i++) {
                LOG("Token: |");
                const string& token = partialParams.at(i);
                LOG(token);
                LOG_END("|");
                if (token.find_first_of("=") == std::string::npos) {
                        error("Bad substitution string: missing '=' in " + token);
                }
                vector<string> pair = split(token, '=');
                if (pair.size() != 2) {
                        error("Bad substitution string: missing separator in " + token);
                }

                VariableConstIterator lb = partialSearchVariable(partialVariables, pair.at(1));

                // map<string, string>::iterator lb = partialVariables.lower_bound(pair.at(0));
                // if (lb != partialVariables.end() && !(partialVariables.key_comp()(pair.at(0), lb->first))) {
                if (lb != partialVariables.end()) {
                        // key already exists
                        // update lb->second if you care to
                        partialVariables.insert(lb, map<string, string>::value_type(pair.at(0), lb->second ));
                } else {
                        // the key does not exist in the map
                        // add it to the map
                        partialVariables.insert(lb, map<string, string>::value_type(pair.at(0), pair.at(1)));
                        // Use lb as a hint to insert,
                        // so it can avoid another lookup
                }
        }

        LOG_END("===============================================");
        for (Tokens::size_type i = 1; i != newTokens.size(); i++) {
                //LOG("  New token: ");
                string tokenToCheck = newTokens.at(i);
                string tokenToCheckPrev = newTokens.at(i-1);
                //LOG_END(tokenToCheck);
                if ((tokenToCheck != TOKEN_START_VARIABLE) &&
                    (tokenToCheck != TOKEN_START_BEGIN_SECTION) &&
                    (tokenToCheck != TOKEN_START_END_SECTION) &&
                    (tokenToCheck != TOKEN_START_IF) &&
                    (tokenToCheck != TOKEN_START_UNLESS) &&
                    (tokenToCheck != TOKEN_START_PARTIAL) &&
                    (tokenToCheck != TOKEN_START_TEMPLATE) &&
                    (tokenToCheck != TOKEN_END)) {

                        // Searching
                        LOG("  Searching: ");
                        LOG(tokenToCheck);

                        VariableIterator lb = partialVariables.find(tokenToCheck);
                        if (lb == partialVariables.end()) {
                                LOG_END(" -> Not Found");
                                continue;
                        }
                        LOG(" -> { ");
                        LOG(lb->first);
                        LOG(" : ");
                        LOG(lb->second);
                        LOG_END(" }");

                        // Substitution
                        LOG("    Substitute: ");
                        string valueToSubstitute = lb->second;
                        LOG(newTokens.at(i));
                        LOG("    at position ");
                        LOG(i);

                        if (valueToSubstitute[0] != '\'' && valueToSubstitute[0] != '\"') {
                                // Simple substitution
                                LOG_END("    with nothing");
                                // newTokens.at(i) = valueToSubstitute;
                        } else {
                                // Substitute literal

                                if (tokenToCheckPrev != TOKEN_START_VARIABLE) {
                                        LOG_END("");
                                        LOG_END("NOT A VARIABLE SKIP LITERAL SUBSTITUTION!");
                                        continue;
                                }

                                if (valueToSubstitute[valueToSubstitute.size() - 1 ] != '\'' && valueToSubstitute[valueToSubstitute.size() - 1] != '\"') {
                                        error("Substitution string " + valueToSubstitute + " not properly closed");
                                }
                                LOG(" with literal ");
                                valueToSubstitute = valueToSubstitute.substr(1, valueToSubstitute.size() - 2);
                                LOG_END(valueToSubstitute);

                                // LOG_END("DUMP BEFORE:");
                                // for (std::vector<string>::size_type j = 0; j != newTokens.size(); j++) {
                                //     LOG(j);
                                //     LOG(" ");
                                //     LOG_END(newTokens.at(j));
                                // }

                                i = i - 1;
                                newTokens.erase(newTokens.begin() + i);
                                newTokens.erase(newTokens.begin() + i);

                                LOG_END("DUMP AFTER:");
                                for (std::vector<string>::size_type j = 0; j != newTokens.size(); j++) {
                                        LOG(j);
                                        LOG(" ");
                                        LOG_END(newTokens.at(j));
                                }

                                LOG("NOW i = ");
                                LOG_END(i);
                        }
                        newTokens.at(i) = valueToSubstitute;
                }
        }
        //partialVariables.clear();
        LOG_END("===============================================");
}

void Mustache::error(const string& message) {
        LOG_END("  [ERROR]");
        throw RenderException(message);
}

json Mustache::searchVariableInContext(const string& key) {
        // TODO: better to use a constant
        if (key == "@index") {
                json value = currentListCounter_;
                return value;
        }
        if (key == "@first") {
                json value = (currentListCounter_ == 0);
                return value;
        }
        // Get the current context
        json top = stack_.top();
        LOG_END("SEARCH CONTEXT:");
        LOG_END(top.dump(2));
        if (top.is_null()) {
                return "null"_json;
        }

        std::string::size_type start;
        std::string::size_type stop;
        std::string::size_type index = std::string::npos;
        string newKey = key;

        if ((start = key.find_first_of("[")) != std::string::npos) {
                if ((stop = key.find_first_of("]")) == std::string::npos) {
                        error("Missing ] in array selection");
                }
                if (start + 1 == stop) {
                        error("Index is empty");
                }
                string array = key.substr(0, start);
                string indexAsString = key.substr(start + 1, stop - 1);

                newKey = array;
                index = std::stoul(indexAsString);
        }

        json::const_iterator it = top.find(newKey);
        if (it == top.end()) {
                LOG_END("NOT FOUND:");
                throw std::invalid_argument("Variable " + key + " not found");
        } else if (index == std::string::npos) {
                LOG_END("NORMAL USE *it:");
                json value = *it;
                return value;
        } else {
                LOG_END("USE INDEX:");
                // top exists at this point
                if (it->is_array()) {
                    LOG_END("USE ARRAY:");
                    if (index >= it->size()) {
                      LOG_END("OUT OF RANGE:");
                      throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
                    } else {
                      LOG_END("IN RANGE:");
                    }
                }
                json value = (*it)[index];
                return value;
        }
}

string Mustache::getTemplateNameFromContext(const string& key)
{
    json variable;
    try {
        variable = searchVariableInContext(key);
    } catch(const std::out_of_range& ex) {
        variable = nullptr;
    } catch(const std::invalid_argument& ex) {
        variable = nullptr;
    }

    if (variable.is_null()) {
        error("Missing template variable: " + key);
    } else if (!variable.is_string()) {
        error("Wrong template variable type: " + key + " must be a string");
    }

    return variable.get<string>();
}

Mustache::VariableConstIterator Mustache::partialSearchVariable(const Mustache::Variables& variables,
        const string& valueToSearch) {
        VariableConstIterator it;
        LOG("  Search in variables: ");
        LOG(valueToSearch);

        if (valueToSearch[0] == '\'' || valueToSearch[0] == '\"') {
                LOG_END("  LITERAL!");
                return variables.end();
        }
        if ((it = variables.find(valueToSearch)) != variables.end()) {
                LOG(" { ");
                LOG(it->first);
                LOG(" : ");
                LOG(it->second);
                LOG_END(" }  YES!");
        } else {
                LOG_END("  NO!");
        }

        return it;
}

void Mustache::ensureValidIdentifier(const string& id, const string& validChars) {
        std::size_t found = id.find_first_not_of(validChars);
        if (found != std::string::npos) {
                error("Invalid identifier '" + id + "': bad char '" + id.at(found) + "' at index " + std::to_string(found) + " valid are " + validChars);
        }
}

vector<string>& Mustache::split(const string &str, char delim, vector<string> &elems) {
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delim)) {
                elems.push_back(item);
        }
        return elems;
}

vector<string> Mustache::split(const string& str, char delim) {
        std::vector<std::string> elems;
        split(str, delim, elems);
        return elems;
}

#ifdef DEBUG

void Mustache::dumpTokens() {
        TokenIndex idx;
        std::cout << "Tokens (size " << tokens_.size() << "): " << std::endl;
        std::cout << "----+-------------------------------------------------" << std::endl;
        for (idx = 0; idx < tokens_.size(); ++idx) {
                if (idx == currentToken_) {
                        std::cout << "==> |" << idx << "|";
                } else {
                        std::cout << "    |" << idx << "|";
                }
                std::cout << tokens_.at(idx) << std::endl;
        }
        std::cout << "----+-------------------------------------------------" << std::endl;
}
#endif

// trim from start
inline string& Mustache::ltrim(string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
inline std::string& Mustache::rtrim(string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
inline string& Mustache::trim(string& s) {
        return ltrim(rtrim(s));
}

void Mustache::htmlEscape(string& data)
{
    std::string buffer;

	auto it = data.cbegin();
	while (it != data.cend()) {
		int sz = 1;
		const uint8_t tchar = *it;

        // From Wikipedia: https://en.wikipedia.org/wiki/UTF-8
        // U+0000  - U+007F   0xxxxxxx
        // U+0080  - U+07FF   110xxxxx  10xxxxxx
        // U+0800  - U+FFFF   1110xxxx  10xxxxxx  10xxxxxx
        // U+10000 - U+10FFFF 11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
		if ((tchar & 0x80) == 0x80) {
            // Multi-byte UTF-8 character
            if ((tchar & 0xC0) == 0xC0) {
                // 2-byte UTF-8 character
                // 110xxxxx
                sz = 2;
            } else if ((tchar & 0xE0) == 0xE0) {
                sz = 3; // 3-byte UTF-8 character
            } else if ((tchar & 0xF8) == 0xF8) {
                sz = 4; // 4-byte UTF-8 character
            }
			for (int cnt = 0; cnt < sz; ++cnt) {
				buffer.push_back(*it++);
			}
		} else { // single-byte character
			switch (tchar) {
			case '&':	buffer.append("&amp;");		break;
			case '\"':	buffer.append("&quot;");	break;
			case '\'':	buffer.append("&apos;");	break;
			case '<':	buffer.append("&lt;");		break;
			case '>':	buffer.append("&gt;");		break;
			case '%':	buffer.append("&percnt;");	break;
			default:
				if (! std::iscntrl(tchar)) {
					buffer.push_back(tchar);
				}
				break;
			}
			++it;
		}
	}
	data.swap(buffer);
}

}  // namespace mustache

////////////////////////////////////////////////////////////////////////////////
