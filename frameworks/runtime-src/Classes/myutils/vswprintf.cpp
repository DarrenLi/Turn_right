//********************************************************************
//	created:	5:9:2012   17:05
//	filename: 	vswprintf.cpp
//	author:		tiamo
//	purpose:	wprintf
//********************************************************************
#include "stdafx.h"
#include "platformhelper.h"

namespace platform
{
#ifdef _WIN32

	#ifndef SIZEOF_SIZE_T
	#  define SIZEOF_SIZE_T (sizeof(size_t))
	#endif

	#if defined(_MSC_VER) && (_MSC_VER >= 900) && (_INTEGRAL_MAX_BITS >= 64)
	#	define LONG_LONG_TYPE __int64
	#	define HAVE_LONG_LONG_TYPE
	#else
	#	undef LONG_LONG_TYPE
	#	undef HAVE_LONG_LONG_TYPE
	#endif

		/*
		* Max integer data types that mprintf.c is capable
		*/

	#ifdef HAVE_LONG_LONG_TYPE
	#  define mp_intmax_t LONG_LONG_TYPE
	#  define mp_uintmax_t unsigned LONG_LONG_TYPE
	#else
	#  define mp_intmax_t long
	#  define mp_uintmax_t unsigned long
	#endif

	#define BUFFSIZE 256 /* buffer for long-to-str and float-to-str calcs */
	#define MAX_PARAMETERS 128 /* lame static limit */


	/* Lower-case digits.  */
	static const wchar_t lower_digits[] = L"0123456789abcdefghijklmnopqrstuvwxyz";

	/* Upper-case digits.  */
	static const wchar_t upper_digits[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

#define OUTCHAR(x) \
	do{ \
	if(stream((wchar_t)(x), (FILE *)data) != -1) \
	done++; \
	else \
	return done; /* return immediately on failure */ \
	} while(0)

	/* Data type to read from the arglist */
	typedef enum  {
		FORMAT_UNKNOWN = 0,
		FORMAT_STRING,
		FORMAT_PTR,
		FORMAT_INT,
		FORMAT_INTPTR,
		FORMAT_LONG,
		FORMAT_LONGLONG,
		FORMAT_DOUBLE,
		FORMAT_LONGDOUBLE,
		FORMAT_WIDTH /* For internal use */
	} FormatType;

	/* convertion and display flags */
	enum {
		FLAGS_NEW        = 0,
		FLAGS_SPACE      = 1<<0,
		FLAGS_SHOWSIGN   = 1<<1,
		FLAGS_LEFT       = 1<<2,
		FLAGS_ALT        = 1<<3,
		FLAGS_SHORT      = 1<<4,
		FLAGS_LONG       = 1<<5,
		FLAGS_LONGLONG   = 1<<6,
		FLAGS_LONGDOUBLE = 1<<7,
		FLAGS_PAD_NIL    = 1<<8,
		FLAGS_UNSIGNED   = 1<<9,
		FLAGS_OCTAL      = 1<<10,
		FLAGS_HEX        = 1<<11,
		FLAGS_UPPER      = 1<<12,
		FLAGS_WIDTH      = 1<<13, /* '*' or '*<num>$' used */
		FLAGS_WIDTHPARAM = 1<<14, /* width PARAMETER was specified */
		FLAGS_PREC       = 1<<15, /* precision was specified */
		FLAGS_PRECPARAM  = 1<<16, /* precision PARAMETER was specified */
		FLAGS_CHAR       = 1<<17, /* %c story */
		FLAGS_FLOATE     = 1<<18, /* %e or %E */
		FLAGS_FLOATG     = 1<<19  /* %g or %G */
	};

	typedef struct {
		FormatType type;
		int flags;
		long width;     /* width OR width parameter number */
		long precision; /* precision OR precision parameter number */
		union {
			wchar_t *str;
			void *ptr;
			union {
				mp_intmax_t as_signed;
				mp_uintmax_t as_unsigned;
			} num;
			double dnum;
		} data;
	} va_stack_t;

	struct nsprintf {
		wchar_t *buffer;
		size_t length;
		size_t max;
	};

#define ISDIGIT(x)  (isdigit((int)  ((unsigned char)x)))
	static long dprintf_DollarString(wchar_t *input, wchar_t **end)
	{
		int number=0;
		while(ISDIGIT(*input)) {
			number *= 10;
			number += *input-L'0';
			input++;
		}
		if(number && (L'$'==*input++)) {
			*end = input;
			return number;
		}
		return 0;
	}

	static int dprintf_IsQualifierNoDollar(wchar_t c)
	{
		switch (c) {
		case '-': case '+': case ' ': case '#': case '.':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case 'h': case 'l': case 'L': case 'z': case 'q':
		case '*': case 'O':
			return 1; /* true */
		default:
			return 0; /* false */
		}
	}

	/******************************************************************
	*
	* Pass 1:
	* Create an index with the type of each parameter entry and its
	* value (may vary in size)
	*
	******************************************************************/

	static long dprintf_Pass1(const wchar_t *format, va_stack_t *vto, wchar_t **endpos,
		va_list arglist)
	{
		wchar_t *fmt = (wchar_t *)format;
		int param_num = 0;
		long this_param;
		long width;
		long precision;
		int flags;
		long max_param=0;
		long i;

		while(*fmt) {
			if(*fmt++ == L'%') {
				if(*fmt == L'%') {
					fmt++;
					continue; /* while */
				}

				flags = FLAGS_NEW;

				/* Handle the positional case (N$) */

				param_num++;

				this_param = dprintf_DollarString(fmt, &fmt);
				if(0 == this_param)
					/* we got no positional, get the next counter */
						this_param = param_num;

				if(this_param > max_param)
					max_param = this_param;

				/*
				* The parameter with number 'i' should be used. Next, we need
				* to get SIZE and TYPE of the parameter. Add the information
				* to our array.
				*/

				width = 0;
				precision = 0;

				/* Handle the flags */

				while(dprintf_IsQualifierNoDollar(*fmt)) {
					switch (*fmt++) {
					case L' ':
						flags |= FLAGS_SPACE;
						break;
					case L'+':
						flags |= FLAGS_SHOWSIGN;
						break;
					case L'-':
						flags |= FLAGS_LEFT;
						flags &= ~FLAGS_PAD_NIL;
						break;
					case L'#':
						flags |= FLAGS_ALT;
						break;
					case L'.':
						flags |= FLAGS_PREC;
						if(L'*' == *fmt) {
							/* The precision is picked from a specified parameter */

							flags |= FLAGS_PRECPARAM;
							fmt++;
							param_num++;

							i = dprintf_DollarString(fmt, &fmt);
							if(i)
								precision = i;
							else
								precision = param_num;

							if(precision > max_param)
								max_param = precision;
						}
						else {
							flags |= FLAGS_PREC;
							precision = wcstol(fmt, &fmt, 10);
						}
						break;
					case L'h':
						flags |= FLAGS_SHORT;
						break;
					case L'l':
						if(flags & FLAGS_LONG)
							flags |= FLAGS_LONGLONG;
						else
							flags |= FLAGS_LONG;
						break;
					case L'L':
						flags |= FLAGS_LONGDOUBLE;
						break;
					case L'q':
						flags |= FLAGS_LONGLONG;
						break;
//					case L'z':
//						/* the code below generates a warning if -Wunreachable-code is
//						used */
//#if (SIZEOF_SIZE_T > sizeof(long))
//						flags |= FLAGS_LONGLONG;
//#else
//						flags |= FLAGS_LONG;
//#endif
//						break;
//					case L'O':
//#if (CURL_SIZEOF_CURL_OFF_T > sizeof(long))
//						flags |= FLAGS_LONGLONG;
//#else
//						flags |= FLAGS_LONG;
//#endif
//						break;
					case L'0':
						if(!(flags & FLAGS_LEFT))
							flags |= FLAGS_PAD_NIL;
						/* FALLTHROUGH */
					case L'1': case L'2': case L'3': case L'4':
					case L'5': case L'6': case L'7': case L'8': case L'9':
						flags |= FLAGS_WIDTH;
						width = wcstol(fmt-1, &fmt, 10);
						break;
					case L'*':  /* Special case */
						flags |= FLAGS_WIDTHPARAM;
						param_num++;

						i = dprintf_DollarString(fmt, &fmt);
						if(i)
							width = i;
						else
							width = param_num;
						if(width > max_param)
							max_param=width;
						break;
					default:
						break;
					}
				} /* switch */

				/* Handle the specifier */

				i = this_param - 1;

				switch (*fmt) {
				case L'S':
					flags |= FLAGS_ALT;
					/* FALLTHROUGH */
				case L's':
					vto[i].type = FORMAT_STRING;
					break;
				case L'n':
					vto[i].type = FORMAT_INTPTR;
					break;
				case L'p':
					vto[i].type = FORMAT_PTR;
					break;
				case L'd': case L'i':
					vto[i].type = FORMAT_INT;
					break;
				case L'u':
					vto[i].type = FORMAT_INT;
					flags |= FLAGS_UNSIGNED;
					break;
				case L'o':
					vto[i].type = FORMAT_INT;
					flags |= FLAGS_OCTAL;
					break;
				case L'x':
					vto[i].type = FORMAT_INT;
					flags |= FLAGS_HEX;
					break;
				case L'X':
					vto[i].type = FORMAT_INT;
					flags |= FLAGS_HEX|FLAGS_UPPER;
					break;
				case L'c':
					vto[i].type = FORMAT_INT;
					flags |= FLAGS_CHAR;
					break;
				case L'f':
					vto[i].type = FORMAT_DOUBLE;
					break;
				case L'e':
					vto[i].type = FORMAT_DOUBLE;
					flags |= FLAGS_FLOATE;
					break;
				case L'E':
					vto[i].type = FORMAT_DOUBLE;
					flags |= FLAGS_FLOATE|FLAGS_UPPER;
					break;
				case L'g':
					vto[i].type = FORMAT_DOUBLE;
					flags |= FLAGS_FLOATG;
					break;
				case L'G':
					vto[i].type = FORMAT_DOUBLE;
					flags |= FLAGS_FLOATG|FLAGS_UPPER;
					break;
				default:
					vto[i].type = FORMAT_UNKNOWN;
					break;
				} /* switch */

				vto[i].flags = flags;
				vto[i].width = width;
				vto[i].precision = precision;

				if(flags & FLAGS_WIDTHPARAM) {
					/* we have the width specified from a parameter, so we make that
					parameter's info setup properly */
					vto[i].width = width - 1;
					i = width - 1;
					vto[i].type = FORMAT_WIDTH;
					vto[i].flags = FLAGS_NEW;
					vto[i].precision = vto[i].width = 0; /* can't use width or precision
															of width! */
				}
				if(flags & FLAGS_PRECPARAM) {
					/* we have the precision specified from a parameter, so we make that
					parameter's info setup properly */
					vto[i].precision = precision - 1;
					i = precision - 1;
					vto[i].type = FORMAT_WIDTH;
					vto[i].flags = FLAGS_NEW;
					vto[i].precision = vto[i].width = 0; /* can't use width or precision
															of width! */
				}
				*endpos++ = fmt + 1; /* end of this sequence */
			}
		}

		/* Read the arg list parameters into our data list */
		for (i=0; i<max_param; i++) {
			if((i + 1 < max_param) && (vto[i + 1].type == FORMAT_WIDTH))
			{
				/* Width/precision arguments must be read before the main argument
				* they are attached to
				*/
				vto[i + 1].data.num.as_signed = (mp_intmax_t)va_arg(arglist, int);
			}

			switch (vto[i].type)
			{
			case FORMAT_STRING:
				vto[i].data.str = va_arg(arglist, wchar_t *);
				break;

			case FORMAT_INTPTR:
			case FORMAT_UNKNOWN:
			case FORMAT_PTR:
				vto[i].data.ptr = va_arg(arglist, void *);
				break;

			case FORMAT_INT:
#ifdef HAVE_LONG_LONG_TYPE
				if((vto[i].flags & FLAGS_LONGLONG) && (vto[i].flags & FLAGS_UNSIGNED))
					vto[i].data.num.as_unsigned =
					(mp_uintmax_t)va_arg(arglist, mp_uintmax_t);
				else if(vto[i].flags & FLAGS_LONGLONG)
					vto[i].data.num.as_signed =
					(mp_intmax_t)va_arg(arglist, mp_intmax_t);
				else
#endif
				{
					if((vto[i].flags & FLAGS_LONG) && (vto[i].flags & FLAGS_UNSIGNED))
						vto[i].data.num.as_unsigned =
						(mp_uintmax_t)va_arg(arglist, unsigned long);
					else if(vto[i].flags & FLAGS_LONG)
						vto[i].data.num.as_signed =
						(mp_intmax_t)va_arg(arglist, long);
					else if(vto[i].flags & FLAGS_UNSIGNED)
						vto[i].data.num.as_unsigned =
						(mp_uintmax_t)va_arg(arglist, unsigned int);
					else
						vto[i].data.num.as_signed =
						(mp_intmax_t)va_arg(arglist, int);
				}
				break;

			case FORMAT_DOUBLE:
				vto[i].data.dnum = va_arg(arglist, double);
				break;

			case FORMAT_WIDTH:
				/* Argument has been read. Silently convert it into an integer
				* for later use
				*/
				vto[i].type = FORMAT_INT;
				break;

			default:
				break;
			}
		}

		return max_param;

	}

	int msnprintf(wchar_t *buffer, size_t maxlength, const wchar_t *format, ...);

	static int dprintf_formatf(
		void *data, /* untouched by format(), just sent to the stream() function in
					the second argument */
					/* function pointer called for each output character */
					int (*stream)(int, FILE *),
					const wchar_t *format,    /* %-formatted string */
					va_list ap_save) /* list of parameters */
	{
		/* Base-36 digits for numbers.  */
		const wchar_t *digits = lower_digits;

		/* Pointer into the format string.  */
		wchar_t *f;

		/* Number of characters written.  */
		int done = 0;

		long param; /* current parameter to read */
		long param_num=0; /* parameter counter */

		va_stack_t vto[MAX_PARAMETERS];
		wchar_t *endpos[MAX_PARAMETERS];
		wchar_t **end;

		wchar_t work[BUFFSIZE];

		va_stack_t *p;

		/* Do the actual %-code parsing */
		dprintf_Pass1(format, vto, endpos, ap_save);

		end = &endpos[0]; /* the initial end-position from the list dprintf_Pass1()
							created for us */

		f = (wchar_t *)format;
		while(*f != L'\0') {
			/* Format spec modifiers.  */
			int is_alt;

			/* Width of a field.  */
			long width;

			/* Precision of a field.  */
			long prec;

			/* Decimal integer is negative.  */
			int is_neg;

			/* Base of a number to be written.  */
			long base;

			/* Integral values to be written.  */
			mp_uintmax_t num;

			/* Used to convert negative in positive.  */
			mp_intmax_t signed_num;

			if(*f != L'%') {
				/* This isn't a format spec, so write everything out until the next one
				OR end of string is reached.  */
				do {
					OUTCHAR(*f);
				} while(*++f && (L'%' != *f));
				continue;
			}

			++f;

			/* Check for "%%".  Note that although the ANSI standard lists
			'%' as a conversion specifier, it says "The complete format
			specification shall be `%%'," so we can avoid all the width
			and precision processing.  */
			if(*f == L'%') {
				++f;
				OUTCHAR(L'%');
				continue;
			}

			/* If this is a positional parameter, the position must follow imediately
			after the %, thus create a %<num>$ sequence */
			param=dprintf_DollarString(f, &f);

			if(!param)
				param = param_num;
			else
				--param;

			param_num++; /* increase this always to allow "%2$s %1$s %s" and then the
							third %s will pick the 3rd argument */

			p = &vto[param];

			/* pick up the specified width */
			if(p->flags & FLAGS_WIDTHPARAM)
				width = (long)vto[p->width].data.num.as_signed;
			else
				width = p->width;

			/* pick up the specified precision */
			if(p->flags & FLAGS_PRECPARAM) {
				prec = (long)vto[p->precision].data.num.as_signed;
				param_num++; /* since the precision is extraced from a parameter, we
								must skip that to get to the next one properly */
			}
			else if(p->flags & FLAGS_PREC)
				prec = p->precision;
			else
				prec = -1;

			is_alt = (p->flags & FLAGS_ALT) ? 1 : 0;

			switch (p->type) {
			case FORMAT_INT:
				num = p->data.num.as_unsigned;
				if(p->flags & FLAGS_CHAR) {
					/* Character.  */
					if(!(p->flags & FLAGS_LEFT))
						while(--width > 0)
							OUTCHAR(L' ');
					OUTCHAR((char) num);
					if(p->flags & FLAGS_LEFT)
						while(--width > 0)
							OUTCHAR(L' ');
					break;
				}
				if(p->flags & FLAGS_UNSIGNED) {
					/* Decimal unsigned integer.  */
					base = 10;
					goto unsigned_number;
				}
				if(p->flags & FLAGS_OCTAL) {
					/* Octal unsigned integer.  */
					base = 8;
					goto unsigned_number;
				}
				if(p->flags & FLAGS_HEX) {
					/* Hexadecimal unsigned integer.  */

					digits = (p->flags & FLAGS_UPPER)? upper_digits : lower_digits;
					base = 16;
					goto unsigned_number;
				}

				/* Decimal integer.  */
				base = 10;

				is_neg = (p->data.num.as_signed < (mp_intmax_t)0) ? 1 : 0;
				if(is_neg) {
					/* signed_num might fail to hold absolute negative minimum by 1 */
					signed_num = p->data.num.as_signed + (mp_intmax_t)1;
					signed_num = -signed_num;
					num = (mp_uintmax_t)signed_num;
					num += (mp_uintmax_t)1;
				}

				goto number;

unsigned_number:
				/* Unsigned number of base BASE.  */
				is_neg = 0;

number:
				/* Number of base BASE.  */
				{
					wchar_t *workend = &work[ARRAYSIZE(work) - 1];
					wchar_t *w;

					/* Supply a default precision if none was given.  */
					if(prec == -1)
						prec = 1;

					/* Put the number in WORK.  */
					w = workend;
					while(num > 0) {
						*w-- = digits[num % base];
						num /= base;
					}
					width -= (long)(workend - w);
					prec -= (long)(workend - w);

					if(is_alt && base == 8 && prec <= 0) {
						*w-- = L'0';
						--width;
					}

					if(prec > 0) {
						width -= prec;
						while(prec-- > 0)
							*w-- = L'0';
					}

					if(is_alt && base == 16)
						width -= 2;

					if(is_neg || (p->flags & FLAGS_SHOWSIGN) || (p->flags & FLAGS_SPACE))
						--width;

					if(!(p->flags & FLAGS_LEFT) && !(p->flags & FLAGS_PAD_NIL))
						while(width-- > 0)
							OUTCHAR(L' ');

					if(is_neg)
						OUTCHAR(L'-');
					else if(p->flags & FLAGS_SHOWSIGN)
						OUTCHAR(L'+');
					else if(p->flags & FLAGS_SPACE)
						OUTCHAR(L' ');

					if(is_alt && base == 16) {
						OUTCHAR(L'0');
						if(p->flags & FLAGS_UPPER)
							OUTCHAR(L'X');
						else
							OUTCHAR(L'x');
					}

					if(!(p->flags & FLAGS_LEFT) && (p->flags & FLAGS_PAD_NIL))
						while(width-- > 0)
							OUTCHAR(L'0');

					/* Write the number.  */
					while(++w <= workend) {
						OUTCHAR(*w);
					}

					if(p->flags & FLAGS_LEFT)
						while(width-- > 0)
							OUTCHAR(L' ');
				}
				break;

			case FORMAT_STRING:
				/* String.  */
				{
					static const wchar_t null[] = L"(nil)";
					const wchar_t *str;
					size_t len;

					str = (wchar_t *) p->data.str;
					if( str == NULL) {
						/* Write null[] if there's space.  */
						if(prec == -1 || prec >= (long) sizeof(null) - 1) {
							str = null;
							len = sizeof(null) - 1;
							/* Disable quotes around (nil) */
							p->flags &= (~FLAGS_ALT);
						}
						else {
							str = L"";
							len = 0;
						}
					}
					else
						len = wcslen(str);

					if(prec != -1 && (size_t) prec < len)
						len = static_cast<size_t>(prec);
					width -= (long)len;

					if(p->flags & FLAGS_ALT)
						OUTCHAR(L'"');

					if(!(p->flags&FLAGS_LEFT))
						while(width-- > 0)
							OUTCHAR(L' ');

					while(len-- > 0)
						OUTCHAR(*str++);
					if(p->flags&FLAGS_LEFT)
						while(width-- > 0)
							OUTCHAR(L' ');

					if(p->flags & FLAGS_ALT)
						OUTCHAR(L'"');
				}
				break;

			case FORMAT_PTR:
				/* Generic pointer.  */
				{
					void *ptr;
					ptr = (void *) p->data.ptr;
					if(ptr != NULL) {
						/* If the pointer is not NULL, write it as a %#x spec.  */
						base = 16;
						digits = (p->flags & FLAGS_UPPER)? upper_digits : lower_digits;
						is_alt = 1;
						num = (size_t) ptr;
						is_neg = 0;
						goto number;
					}
					else {
						/* Write "(nil)" for a nil pointer.  */
						static const wchar_t strnil[] = L"(nil)";
						const wchar_t *point;

						width -= sizeof(strnil) - 1;
						if(p->flags & FLAGS_LEFT)
							while(width-- > 0)
								OUTCHAR(L' ');
						for (point = strnil; *point != L'\0'; ++point)
							OUTCHAR(*point);
						if(! (p->flags & FLAGS_LEFT))
							while(width-- > 0)
								OUTCHAR(L' ');
					}
				}
				break;

			case FORMAT_DOUBLE:
				{
					wchar_t formatbuf[32]=L"%";
					wchar_t *fptr;
					size_t left = sizeof(formatbuf)-wcslen(formatbuf);
					int len;

					width = -1;
					if(p->flags & FLAGS_WIDTH)
						width = p->width;
					else if(p->flags & FLAGS_WIDTHPARAM)
						width = (long)vto[p->width].data.num.as_signed;

					prec = -1;
					if(p->flags & FLAGS_PREC)
						prec = p->precision;
					else if(p->flags & FLAGS_PRECPARAM)
						prec = (long)vto[p->precision].data.num.as_signed;

					if(p->flags & FLAGS_LEFT)
						wcscat_s(formatbuf, 32, L"-");
					if(p->flags & FLAGS_SHOWSIGN)
						wcscat_s(formatbuf, 32, L"+");
					if(p->flags & FLAGS_SPACE)
						wcscat_s(formatbuf, 32, L" ");
					if(p->flags & FLAGS_ALT)
						wcscat_s(formatbuf, 32, L"#");

					fptr=&formatbuf[wcslen(formatbuf)];

					if(width >= 0) {
						/* RECURSIVE USAGE */

						len = msnprintf(fptr, left, L"%ld", width);
						fptr += len;
						left -= len;
					}
					if(prec >= 0) {
						/* RECURSIVE USAGE */
						len = msnprintf(fptr, left, L".%ld", prec);
						fptr += len;
						left -= len;
					}
					if(p->flags & FLAGS_LONG)
						*fptr++ = L'l';

					if(p->flags & FLAGS_FLOATE)
						*fptr++ = (wchar_t)((p->flags & FLAGS_UPPER) ? L'E' : L'e');
					else if(p->flags & FLAGS_FLOATG)
						*fptr++ = (wchar_t)((p->flags & FLAGS_UPPER) ? L'G' : L'g');
					else
						*fptr++ = L'f';

					*fptr = 0; /* and a final zero termination */

					/* NOTE NOTE NOTE!! Not all sprintf() implementations returns number
					of output characters */
					platform::snwprintf(work, BUFFSIZE, formatbuf, p->data.dnum);

					for(fptr=work; *fptr; fptr++)
						OUTCHAR(*fptr);
				}
				break;

			case FORMAT_INTPTR:
				/* Answer the count of characters written.  */
#ifdef HAVE_LONG_LONG_TYPE
				if(p->flags & FLAGS_LONGLONG)
					*(LONG_LONG_TYPE *) p->data.ptr = (LONG_LONG_TYPE)done;
				else
#endif
					if(p->flags & FLAGS_LONG)
						*(long *) p->data.ptr = (long)done;
					else if(!(p->flags & FLAGS_SHORT))
						*(int *) p->data.ptr = (int)done;
					else
						*(short *) p->data.ptr = (short)done;
					break;

			default:
				break;
			}
			f = *end++; /* goto end of %-code */

		}
		return done;
	}

	/* fputc() look-alike */
	static int addbyter(int output, FILE *data)
	{
		struct nsprintf *infop=(struct nsprintf *)data;
		wchar_t outc = (wchar_t)output;

		if(infop->length < infop->max) {
			/* only do this if we haven't reached max length yet */
			infop->buffer[0] = outc; /* store */
			infop->buffer++; /* increase pointer */
			infop->length++; /* we are now one byte larger */
			return outc;     /* fputc() returns like this on success */
		}
		return -1;
	}

	int mvsnprintf(wchar_t *buffer, size_t maxlength, const wchar_t *format,
		va_list ap_save)
	{
		int retcode;
		struct nsprintf info;

		info.buffer = buffer;
		info.length = 0;
		info.max = maxlength;

		retcode = dprintf_formatf(&info, addbyter, format, ap_save);
		if(info.max) {
			/* we terminate this with a zero byte */
			if(info.max == info.length)
				/* we're at maximum, scrap the last letter */
					info.buffer[-1] = 0;
			else
				info.buffer[0] = 0;
		}
		return retcode;
	}

	int msnprintf(wchar_t *buffer, size_t maxlength, const wchar_t *format, ...)
	{
		int retcode;
		va_list ap_save; /* argument pointer */
		va_start(ap_save, format);
		retcode = mvsnprintf(buffer, maxlength, format, ap_save);
		va_end(ap_save);
		return retcode;
	}
#endif
}
