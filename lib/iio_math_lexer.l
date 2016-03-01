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

%%

x {
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
