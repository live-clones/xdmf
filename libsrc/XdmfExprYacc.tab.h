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
#define	INDEX	272
#define	EQEQ	273
#define	LT	274
#define	LE	275
#define	GT	276
#define	GE	277
#define	NE	278
#define	LTLT	279
#define	GTGT	280
#define	JOIN	281


extern YYSTYPE dice_yylval;
