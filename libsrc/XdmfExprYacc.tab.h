typedef union {
	double		DoubleValue;
	long		IntegerValue;
	void		*ArrayPointer;
	XdmfExprSymbol	*Symbol;
} YYSTYPE;
#define	lFLOAT	257
#define	INTEGER	258
#define	ARRAY	259
#define	NAME	260
#define	SIN	261
#define	COS	262
#define	TAN	263
#define	ACOS	264
#define	ASIN	265
#define	ATAN	266
#define	LOG	267
#define	EXP	268
#define	ABS_TOKEN	269
#define	SQRT	270
#define	WHERE	271
#define	EQEQ	272
#define	LT	273
#define	LE	274
#define	GT	275
#define	GE	276
#define	NE	277
#define	LTLT	278
#define	GTGT	279
#define	JOIN	280


extern YYSTYPE dice_yylval;
