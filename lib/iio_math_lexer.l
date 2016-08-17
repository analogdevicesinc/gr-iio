%{
/* 
 * Copyright 2016 Analog Devices Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "parser.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
%}

%option noyywrap reentrant bison-bridge nounistd always-interactive

%s WANT_IN_PORT
%s WANT_MULT
%s WANT_TWO_PI
%s WANT_END

%%

<INITIAL>w {
	yyless(0);
	BEGIN(WANT_IN_PORT);
	return '(';
}

<WANT_IN_PORT>w {
	yyless(0);
	BEGIN(WANT_MULT);
	return IN_PORT;
}

<WANT_MULT>w {
	yyless(0);
	BEGIN(WANT_TWO_PI);
	return '*';
}

<WANT_TWO_PI>w {
	yyless(0);
	yylval->val = 2.0 * M_PI;
	BEGIN(WANT_END);
	return VALUE;
}

<WANT_END>w {
	BEGIN(0);
	return ')';
}

(x[0-9])|(t[0-9]) {
	yylval->ival = yytext[1] - '0';
	return IN_PORT;
}

x|t {
	yylval->ival = 0;
	return IN_PORT;
}

e {
	yylval->val = M_E;
	return VALUE;
}

pi {
	yylval->val = M_PI;
	return VALUE;
}

(\*\*)|^ {
	return POWER;
}

[0-9]+([,.][0-9]+)? {
	yylval->val = atof(yytext);
	return VALUE;
}

[a-z]+[0-9]* {
	yylval->fname = strdup(yytext);
	return FNAME;
}

[ \t]+ {
}

. {
	return *yytext;
}
