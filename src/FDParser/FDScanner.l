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
# include <cstring> // strerror
# include <string>
# include "FDParser.hh"
# include "FDParserPrivate.h"
%}

%{
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

%option noyywrap nounput noinput batch nounistd never-interactive

%{
    // A number symbol corresponding to the value in S.
    yy::parser::symbol_type make_NUMBER (const std::string &s, const yy::parser::location_type& loc);
    yy::parser::symbol_type make_INTEGER_CONSTANT (const std::string &s, const yy::parser::location_type& loc,uint32_t base);
    YY_BUFFER_STATE string_buffer;
    #define DECIMAL 10U
    #define HEX 16U
    #define BINARY 2U
%}

D			([0-9])
L			([a-zA-Z_])
H			[a-fA-F0-9]
W           ({L}({L}|{D})*)
PATH		([\/]?({L}|{D})+[-]?\/)
E			([Ee][+-]?{D}+)
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
    yy::location& loc = drv.location;
    // Code run each time yylex is called.
    loc.step ();
    
%}

 /*  COMMENT mode  */
"/*"			{
    loc.step();
    BEGIN(COMMENT);
}
<COMMENT>"*/"		{
    loc.step();
    BEGIN(INITIAL);
}
<COMMENT>{blank}+       loc.step();
<COMMENT>\n+            loc.lines(yyleng); loc.step ();
<COMMENT>.              loc.step();

 /*  Operators  */
"{"                     loc.step(); return yy::parser::make_LBRAN  (loc);
"}"                     loc.step(); return yy::parser::make_RBRAN  (loc);
"["                     loc.step(); return yy::parser::make_LSQUR  (loc);
"]"                     loc.step(); return yy::parser::make_RSQUR  (loc);
"("                     loc.step(); return yy::parser::make_LPAREN  (loc);
")"                     loc.step(); return yy::parser::make_RPAREN  (loc);
","		                loc.step(); return yy::parser::make_COMMA  (loc);
":"		                loc.step(); return yy::parser::make_COLON  (loc);
"&"                     loc.step(); return yy::parser::make_REF  (loc);
";"                     loc.step(); return yy::parser::make_SEMICOLON (loc);

"!"		                loc.step(); return yy::parser::make_EXCL  (loc);
"~"		                loc.step(); return yy::parser::make_WAVE  (loc);
"%"		                loc.step(); return yy::parser::make_PERCENT  (loc);
"^"		                loc.step(); return yy::parser::make_POW  (loc);
"|"		                loc.step(); return yy::parser::make_OR  (loc);
"?"		                loc.step(); return yy::parser::make_QUE  (loc);
"=>"                    loc.step(); return yy::parser::make_ARRAW  (loc);
"->"                    loc.step(); return yy::parser::make_POINT (loc);

"-"                     loc.step(); return yy::parser::make_MINUS (loc);
"+"                     loc.step(); return yy::parser::make_PLUS (loc);
"="                     loc.step(); return yy::parser::make_ASSIGNMENT (loc);


 /*  Key words  */
"package"		        loc.step(); return yy::parser::make_PACKAGE	(loc);
"import"	            loc.step(); return yy::parser::make_IMPORT(loc);

"specification"         loc.step(); return yy::parser::make_SPEC(loc);
"extends"		        loc.step(); return yy::parser::make_EXTENDS (loc);
"for"                   loc.step(); return yy::parser::make_FOR(loc);
"array"                 loc.step(); return yy::parser::make_ARRAY(loc);
 
 /* FDBuiltInPropertyHost */
"type_collections"      loc.step(); return yy::parser::make_PROPERTYHOST(FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::TYPE_COLLECTIONS),loc);
"interfaces"            loc.step(); return yy::parser::make_PROPERTYHOST(FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::INTERFACES),loc);
"attributes"            loc.step(); return yy::parser::make_PROPERTYHOST(FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::ATTRIBUTES),loc);
"methods"               loc.step(); return yy::parser::make_PROPERTYHOST(FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::METHODS),loc);
"broadcasts"            loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::BROADCASTS),loc);
"arguments"             loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::ARGUMENTS),loc);
"struct_fields"         loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::STRUCT_FIELDS),loc);
"union_fields"          loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::UNION_FIELDS),loc);
"arrays"                loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::ARRAYS),loc);
"structs"               loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::STRUCTS),loc);
"unions"                loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::UNIONS),loc);
"enumerations"          loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::ENUMERATIONS),loc);
"enumerators"           loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::ENUMERATORS),loc);
"typedefs"              loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::TYPEDEFS),loc);
"strings"               loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::STRINGS),loc);
"numbers"               loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::NUMBERS),loc);
"integers"              loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::INTEGERS),loc);
"floats"                loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::FLOATS),loc);
"booleans"              loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::BOOLEANS),loc);
"byte_buffers"          loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::BYTE_BUFFERS),loc);
"fields"                loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::FIELDS),loc);
"maps"                  loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::MAPS),loc);
"map_keys"              loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::MAP_KEYS),loc);
"map_values"            loc.step(); return yy::parser::make_PROPERTYHOST (FDBuiltInPropertyHost::get(FDBuiltInPropertyHost::MAP_VALUES),loc);

 /*FDPredefinedTypeId*/
"Integer"               loc.step(); return yy::parser::make_PREDEFTYPE (FDPredefinedTypeId::get(FDPredefinedTypeId::INTEGER),loc);
"String"                loc.step(); return yy::parser::make_PREDEFTYPE (FDPredefinedTypeId::get(FDPredefinedTypeId::STRING),loc);
"Boolean"               loc.step(); return yy::parser::make_PREDEFTYPE (FDPredefinedTypeId::get(FDPredefinedTypeId::BOOLEAN),loc);
"Interface"             loc.step(); return yy::parser::make_PREDEFTYPE (FDPredefinedTypeId::get(FDPredefinedTypeId::INTERFACE),loc);

 /*(FDPropertyFlag,...)* Port: Integer (optional);*/
"optional"              loc.step(); return yy::parser::make_OPTIONAL (loc);
"default"               loc.step(); return yy::parser::make_DEFAULT (loc);

"define"                loc.step(); return yy::parser::make_DEFINE (loc);
"as"                    loc.step(); return yy::parser::make_AS (loc);
"use"                   loc.step(); return yy::parser::make_USE (loc);

 /*FDTypeOverwrites:*/
"#struct"               loc.step(); return yy::parser::make_STRUCTOVERWRITES (loc);
"#union"                loc.step(); return yy::parser::make_UNIONOVERWRITES (loc);
"#enumeration"          loc.step(); return yy::parser::make_ENUMOVERWRITES (loc);
"#"                     loc.step(); return yy::parser::make_HASH_SIGN (loc);

"struct"                loc.step(); return yy::parser::make_STRUCT (loc);
"union"                 loc.step(); return yy::parser::make_UNION (loc);
"enumeration"           loc.step(); return yy::parser::make_ENUMERATION (loc);
"map"                   loc.step(); return yy::parser::make_MAP (loc);
"key"                   loc.step(); return yy::parser::make_KEY (loc);
"value"                 loc.step(); return yy::parser::make_VALUE (loc);
"typedef"               loc.step(); return yy::parser::make_TYPEDEF (loc);
"attribute"             loc.step(); return yy::parser::make_ATTRIBUTE(loc);
"interface"             loc.step(); return yy::parser::make_INTERFACE(loc);
"method"                loc.step(); return yy::parser::make_METHOD(loc);
"broadcast"             loc.step(); return yy::parser::make_BROADCAST(loc);
"instance"              loc.step(); return yy::parser::make_INSTANCE(loc);

"in"                    loc.step(); return yy::parser::make_IN(loc);
"out"                   loc.step(); return yy::parser::make_OUT(loc);

 /*FDTypes*/
"typeCollection"        loc.step(); return yy::parser::make_TYPECOLLECTION (loc);


 /*  Constants  */
(\"(\\.|[^\\\"])*\")|(\'(\\.|[^\\\'])*\')   {
    char *end=yytext+strlen(yytext)-1;
    *end='\0';
    loc.step(); 
    return yy::parser::make_STRING (yytext+1,loc);
} 

"true"		            loc.step(); return yy::parser::make_BOOL_CONSTANT (true,loc);
"false"		            loc.step(); return yy::parser::make_BOOL_CONSTANT (false,loc);

0[bB][01]+		        loc.step(); return make_INTEGER_CONSTANT (yytext+2, loc,BINARY);
{int}      		        loc.step(); return make_INTEGER_CONSTANT (yytext, loc,DECIMAL);
0[xX]{H}+		        loc.step(); return make_INTEGER_CONSTANT (yytext+2, loc,HEX);

{D}*"."{D}+{E}?"d"	{
    char *end=strlen(yytext)+yytext-1;
    *end='\0';
    loc.step(); 
    return yy::parser::make_DOUBLE_CONSTANT (atof(yytext),loc);
}
{D}*"."{D}+{E}?"f"	{
    char *end=strlen(yytext)+yytext-1;
    *end='\0';
    loc.step(); 
    return yy::parser::make_FLOAT_CONSTANT ((float)atof(yytext),loc);
}

 /*  Identifiers  */
{id}	                loc.step(); return yy::parser::make_ID (yytext,loc);
{id}(\.{id})+           loc.step(); return yy::parser::make_FQN (yytext,loc);
{id}(\.{id})+\.\*       loc.step(); return yy::parser::make_IMPORTED_FQN (yytext,loc);
{id}(\.{id})*":"{id}    loc.step(); return yy::parser::make_FQN_WITH_SELECTOR (yytext,loc);

 /*  Skiped words  */
\/\/[^\n\r]*            loc.step ();
{blank}+                loc.step ();
\n                      loc.lines (yyleng); loc.step ();

 /*  Error words  */
.   {
    throw yy::parser::syntax_error
    (loc, "invalid character: " + std::string(yytext));
}

 /*  End of frame  */
<<EOF>>       {
	return yy::parser::make_YYEOF (loc);}
%%

yy::parser::symbol_type make_INTEGER_CONSTANT (const std::string &s, const yy::parser::location_type& loc,uint32_t base)
{
    errno = 0;
    auto n =strtoll (s.c_str(), NULL, base);
    if (! (0 <= n && n <= UINT_MAX && errno != ERANGE))
	    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
	  return yy::parser::make_INTEGER_CONSTANT (n, loc);
}

namespace BstIdl
{

void FDParserPrivate::scan_begin ()
{
	yy_flex_debug = trace_scanning;
	if (file.empty () || (!(yyin = fopen (file.c_str (), "r"))))
	{
		std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
		exit (EXIT_FAILURE);
	}
}

void FDParserPrivate::scan_end ()
{
	fclose (yyin);
    yylex_destroy();
}

void FDParserPrivate::scan_string_begin(const std::string &raw)
{
    string_buffer = yy_scan_string(raw.c_str());
}

void FDParserPrivate::scan_string_end()
{
    yy_delete_buffer(string_buffer);
    yylex_destroy();
}

}

