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

	#include <math.h>
	#include <stdio.h>

	int yylex(YYSTYPE *type, yyscan_t scanner);
	void yyerror(yyscan_t scanner, const char *msg);
%}

%code requires {
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif

int yylex_init_extra(void *pdata, yyscan_t *scanner);
int yylex_destroy(yyscan_t yyscanner);

void * yyget_extra(yyscan_t scanner);

void * src_block(void *pdata, unsigned int input);
void * const_block(void *pdata, double value);
void * add_block(void *pdata, void *left, void *right);
void * sub_block(void *pdata, void *left, void *right);
void * mult_block(void *pdata, void *left, void *right);
void * div_block(void *pdata, void *left, void *right);
void * pow_block(void *pdata, void *left, void *right);
void * mod_block(void *pdata, void *left, void *right);
void * func_block(void *pdata, void *input, const char *name);
void * neg_block(void *pdata, void *input);

void connect_to_output(void *pdata, void *input);
void delete_block(void *pdata, void *block);
}

/* Bison declarations.  */
%define api.pure
%lex-param { yyscan_t scanner }
%parse-param { yyscan_t scanner }

%start Line

%union {
	double val;
	unsigned int ival;
	char *fname;
	void *block;
}

%token<fname> FNAME;
%token<val> VALUE;
%token<ival> IN_PORT;
%type<block> Element;
%type<block> Factor;
%type<block> Term;
%token POWER;

%left '-' '+' '*' '/' '%'
%precedence PREFIX
%right POWER

%destructor { free($$); } <fname>
%destructor { delete_block(yyget_extra(scanner), $$); } <block>

%%

Element:
	IN_PORT[t] {
		$$ = src_block(yyget_extra(scanner), $t);
	}
	| VALUE[t] {
		$$ = const_block(yyget_extra(scanner), $t);
	}
	| FNAME[n] '(' Term[t] ')' {
		$$ = func_block(yyget_extra(scanner), $t, $n);
	}
	| '(' Term[t] ')' {
		$$ = $t;
	}
	| '-' Element[t] %prec PREFIX {
		$$ = neg_block(yyget_extra(scanner), $t);
	}
	| '+' Element[t] %prec PREFIX {
		$$ = $t;
	}
	;

Factor:
	Element
	| Factor[f1] '*' Factor[f2] {
		$$ = mult_block(yyget_extra(scanner), $f1, $f2);
	}
	| Factor[f1] '/' Factor[f2] {
		$$ = div_block(yyget_extra(scanner), $f1, $f2);
	}
	| Factor[f1] POWER Factor[f2] {
		$$ = pow_block(yyget_extra(scanner), $f1, $f2);
	}
	| Factor[f1] '%' Factor[f2] {
		$$ = mod_block(yyget_extra(scanner), $f1, $f2);
		/* TODO(pcercuei): Implement modulo */
	}
	;

Term:
	Factor
	/* TODO(pcercuei): Implement const operations
	| Term '+' VALUE { }
	| Term '-' VALUE { }
	| VALUE '+' Term { }
	| VALUE '-' Term { }
	*/
	| Term[t1] '+' Term[t2] {
		$$ = add_block(yyget_extra(scanner), $t1, $t2);
	}
	| Term[t1] '-' Term[t2] {
		$$ = sub_block(yyget_extra(scanner), $t1, $t2);
	}
	;

Line:
	Term[t] {
		connect_to_output(yyget_extra(scanner), $[t]);
	}

%%

void yyerror(yyscan_t scanner, const char *msg)
{
	printf("ERROR: %s\n", msg);
}
