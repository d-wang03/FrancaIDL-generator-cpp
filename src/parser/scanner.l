/*
 * Copyright (c) 2024 Black Sesame Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
%{ 
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring>
# include <string>
# include "parser.hh"
# include "FidlParserPrivate.h"
# include "utilities/string_utility.h"
%}

%{
#if defined __clang__
# define CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#endif

// Clang and ICC like to pretend they are GCC.
#if defined __GNUC__ && !defined __clang__ && !defined __ICC
# define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#endif

// Pacify warnings in yy_init_buffer (observed with Flex 2.6.4)
// and GCC 6.4.0, 7.3.0 with -O3.
#if defined GCC_VERSION && 600 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wnull-dereference"
#endif

// This example uses Flex's C back end, yet compiles it as C++.
// So expect warnings about C style casts and NULL.
#if defined CLANG_VERSION && 500 <= CLANG_VERSION
# pragma clang diagnostic ignored "-Wold-style-cast"
# pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#elif defined GCC_VERSION && 407 <= GCC_VERSION
# pragma GCC diagnostic ignored "-Wold-style-cast"
# pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif

%}

%option noyywrap nounput noinput batch
%option prefix="bst"
%{
    // A number symbol corresponding to the value in S.
    bst::parser::symbol_type make_NUMBER (const std::string &s, const bst::parser::location_type& loc);
    bst::parser::symbol_type make_INTEGER_CONSTANT (const std::string &s, const bst::parser::location_type& loc,uint32_t base);
    YY_BUFFER_STATE string_buffer_;
    #define DECIMAL 10U
    #define HEX 16U
    #define BINARY 2U
%}

D            ([0-9])
L            ([a-zA-Z_])
H            [a-fA-F0-9]
W           ({L}({L}|{D})*)
PATH        ([\/]?({L}|{D})+[-]?\/)
E            ([Ee][+-]?{D}+)
FS          (f|d)
id          [\^]?{W}
int         [0-9]+
blank       [ \t\r]

%x COMMENT
%{
    // Code run each time a pattern is matched.
    # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
    std::string annotation;
    // A handy shortcut to the location held by the driver.
    bst::location& loc = drv.location;
    // Code run each time yylex is called.
    loc.step ();
%}

 /*  COMMENT mode  */
"/*"            {
    loc.step();
    BEGIN(COMMENT);
}
<COMMENT>"*/"        {
    loc.step();
    BEGIN(INITIAL);
}
<COMMENT>{blank}+       loc.step ();
<COMMENT>\n+            loc.lines (yyleng); loc.step ();
<COMMENT>.              loc.step();


 /*  Annotations  */
"<\*\*"                 loc.step(); return bst::parser::make_LCOMMENT  (loc);
"\*\*>"                 loc.step(); return bst::parser::make_RCOMMENT  (loc);
@[a-z\-]+[ \t]*\:((\\.)|[^\\\*\@])*   {
    loc.lines(count(yytext, "\n"));
    return bst::parser::make_ANNOTATION (yytext,loc);
}
 /*  Operators  */
"{"                     loc.step(); return bst::parser::make_LBRAN  (loc);
"}"                     loc.step(); return bst::parser::make_RBRAN  (loc);
"["                     loc.step(); return bst::parser::make_LSQUR  (loc);
"]"                     loc.step(); return bst::parser::make_RSQUR  (loc);
"("                     loc.step(); return bst::parser::make_LPAREN  (loc);
")"                     loc.step(); return bst::parser::make_RPAREN  (loc);
"="                     loc.step(); return bst::parser::make_ASSIGN  (loc);
"+"                     loc.step(); return bst::parser::make_PLUS  (loc);
"-"                     loc.step(); return bst::parser::make_MINUS  (loc);
"*"                     loc.step(); return bst::parser::make_STAR  (loc);
"/"                     loc.step(); return bst::parser::make_SLASH  (loc);
"<="                    loc.step(); return bst::parser::make_LE  (loc);
">="                    loc.step(); return bst::parser::make_GE  (loc);
"=="                    loc.step(); return bst::parser::make_EQ  (loc);
"!="                    loc.step(); return bst::parser::make_NE  (loc);
","                        loc.step(); return bst::parser::make_COMMA  (loc);
":"                        loc.step(); return bst::parser::make_COLON  (loc);
"!"                        loc.step(); return bst::parser::make_EXCL  (loc);
"~"                        loc.step(); return bst::parser::make_WAVE  (loc);
"%"                        loc.step(); return bst::parser::make_PERCENT  (loc);
"<"                        loc.step(); return bst::parser::make_LESS  (loc);
">"                        loc.step(); return bst::parser::make_GREATER  (loc);
"^"                        loc.step(); return bst::parser::make_POW  (loc);
"||"                    loc.step(); return bst::parser::make_OR  (loc);
"?"                        loc.step(); return bst::parser::make_QUE  (loc);
"=>"                    loc.step(); return bst::parser::make_ARRAW  (loc);
"&&"                    loc.step(); return bst::parser::make_AND  (loc);
"->"                    loc.step(); return bst::parser::make_POINT (loc);

 /*  Key words  */
"package"                loc.step(); return bst::parser::make_PACKAGE    (loc);

"import"                loc.step(); return bst::parser::make_IMPORT(loc);
"model"                 loc.step(); return bst::parser::make_MODEL(loc);
"from"                  loc.step(); return bst::parser::make_FROM(loc);

"interface"                loc.step(); return bst::parser::make_INTERFACE (loc);
"extends"                loc.step(); return bst::parser::make_EXTENDS (loc);
"manages"                loc.step(); return bst::parser::make_MANAGES (loc);

"version"                loc.step(); return bst::parser::make_VERSION (loc);
"major"                 loc.step(); return bst::parser::make_MAJOR (loc);
"minor"                 loc.step(); return bst::parser::make_MINOR (loc);

"attribute"             loc.step(); return bst::parser::make_ATTRIBUTE (loc);
"readonly"              loc.step(); return bst::parser::make_READONLY (loc);
"noRead"                loc.step(); return bst::parser::make_NOREAD (loc);
"noSubscriptions"       loc.step(); return bst::parser::make_NOSUBS (loc);

"method"                loc.step(); return bst::parser::make_METHOD (loc);
"fireAndForget"         loc.step(); return bst::parser::make_FORGET (loc);
"in"                    loc.step(); return bst::parser::make_IN (loc);
"out"                    loc.step(); return bst::parser::make_OUT (loc);
"error"                    loc.step(); return bst::parser::make_ERROR (loc);
"broadcast"                loc.step(); return bst::parser::make_BROADCAST (loc);
"selective"                loc.step(); return bst::parser::make_SELECTIVE (loc);
"typeCollection"        loc.step(); return bst::parser::make_TYPECOLLECTION (loc);

"struct"                loc.step(); return bst::parser::make_STRUCT (loc);
"polymorphic"           loc.step(); return bst::parser::make_POLY (loc);

"enumeration"           loc.step(); return bst::parser::make_ENUM (loc);

"map"                    loc.step(); return bst::parser::make_MAP (loc);
"to"                    loc.step(); return bst::parser::make_TO (loc);

"array"                 loc.step(); return bst::parser::make_ARRAY (loc);
"of"                    loc.step(); return bst::parser::make_OF (loc);

"typedef"                loc.step(); return bst::parser::make_TYPEDEF (loc);
"is"                    loc.step(); return bst::parser::make_IS (loc);
"public"                loc.step(); return bst::parser::make_PUBLIC (loc);

"union"                    loc.step(); return bst::parser::make_UNION (loc);
"Integer"                loc.step(); return bst::parser::make_INTEGER (loc);
"minInt"                loc.step(); return bst::parser::make_MININT (loc);
"maxInt"                loc.step(); return bst::parser::make_MAXINT (loc);
"const"                 loc.step(); return bst::parser::make_CONST (loc);

"errordef"              loc.step(); return bst::parser::make_ERRORDEF (loc);
"errorval"              loc.step(); return bst::parser::make_ERRORVAL (loc);

 /*  Predeined data types  */
"UInt8"                    loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::UINT8),loc);
"Int8"                    loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::INT8),loc);
"UInt16"                loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::UINT16),loc);
"Int16"                    loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::INT16),loc);
"UInt32"                loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::UINT32),loc);
"Int32"                    loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::INT32),loc);
"UInt64"                loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::UINT64),loc);
"Int64"                    loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::INT64),loc);
"Boolean"                loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::BOOLEAN),loc);
"Float"                    loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::FLOAT),loc);
"Double"                loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::DOUBLE),loc);
"String"                loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::STRING),loc);
"ByteBuffer"            loc.step(); return bst::parser::make_BASIC_TYPE (FBasicTypeId::get(FBasicTypeId::BYTE_BUFFER),loc);

 /*  Constants  */
(\"(\\.|[^\\\"])*\")|(\'(\\.|[^\\\'])*\')   {
    char *end=yytext+strlen(yytext)-1;
    *end='\0';
    loc.step(); 
    return bst::parser::make_STRING (yytext+1,loc);
} 

"true"                    loc.step(); return bst::parser::make_BOOL_CONSTANT (std::make_shared<FBooleanConstant>(true),loc);
"false"                    loc.step(); return bst::parser::make_BOOL_CONSTANT (std::make_shared<FBooleanConstant>(false),loc);

0[bB][01]+                loc.step(); return make_INTEGER_CONSTANT (yytext+2, loc,BINARY);
{int}                      loc.step(); return make_INTEGER_CONSTANT (yytext, loc,DECIMAL);
0[xX]{H}+                loc.step(); return make_INTEGER_CONSTANT (yytext+2, loc,HEX);

{D}*"."{D}+{E}?"d"    {
    char *end=strlen(yytext)+yytext-1;
    *end='\0';
    loc.step();
    return bst::parser::make_DOUBLE_CONSTANT (std::make_shared<FDoubleConstant>(atof(yytext)),loc);
}
{D}*"."{D}+{E}?"f"    {
    char *end=strlen(yytext)+yytext-1;
    *end='\0';
    loc.step();
    return bst::parser::make_FLOAT_CONSTANT (std::make_shared<FFloatConstant>((float)atof(yytext)),loc);
}

 /*  Identifiers  */
{id}                    loc.step(); return bst::parser::make_ID (yytext,loc);
{id}(\.{id})+           loc.step(); return bst::parser::make_FQN (yytext,loc);
{id}(\.{id})+\.\*       loc.step(); return bst::parser::make_IMPORTED_FQN (yytext,loc);

 /*  Skiped words  */
\/\/[^\n\r]*            loc.step ();
{blank}+                loc.step ();
\n+                     loc.lines (yyleng); loc.step ();

 /*  Error words  */
.   {
    throw bst::parser::syntax_error
    (loc, "invalid character: " + std::string(yytext));
}

 /*  End of frame  */
<<EOF>>       {
    return bst::parser::make_BSTEOF (loc);}
%%

bst::parser::symbol_type make_INTEGER_CONSTANT (const std::string &s, const bst::parser::location_type& loc,uint32_t base)
{
    errno = 0;
    auto n =strtoll (s.c_str(), NULL, base);
    if (! (0 <= n && n <= UINT_MAX && errno != ERANGE))
        throw bst::parser::syntax_error (loc, "integer is out of range: " + s);
    auto obj = FFactory::getInstance().createFIntegerConstant();
    obj->setUnsignedVal(n);
      return bst::parser::make_INTEGER_CONSTANT (obj, loc);
}

namespace BstIdl
{

void FidlParserPrivate::scan_begin ()
{
    yy_flex_debug = trace_scanning;
    if (filename.empty () || (!(yyin = fopen (filename.c_str (), "r"))))
    {
        std::cerr << "cannot open " << filename << ": " << strerror (errno) << '\n';
        exit (EXIT_FAILURE);
    }
}

void FidlParserPrivate::scan_end ()
{
    fclose (yyin);
    yylex_destroy();
}

void FidlParserPrivate::scan_string_begin(const std::string &raw)
{
    string_buffer_ = yy_scan_string(raw.c_str());
}

void FidlParserPrivate::scan_string_end()
{
    yy_delete_buffer(string_buffer_);
    yylex_destroy();
}

}

