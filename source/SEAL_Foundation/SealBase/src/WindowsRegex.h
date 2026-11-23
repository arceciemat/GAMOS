#ifndef SEAL_BASE_WINDOWS_REGEX_H
# define SEAL_BASE_WINDOWS_REGEX_H

// Minimal POSIX <regex.h> replacement for Windows/MSVC, which has no
// POSIX regex library.  Implemented on top of std::regex and covering
// only what SealBase/src/Regexp.cc uses: regcomp, regexec, regfree,
// regerror and the flags REG_EXTENDED, REG_ICASE, REG_NEWLINE,
// REG_NOTBOL, REG_NOTEOL.
//
// Patterns compiled with REG_EXTENDED use std::regex::extended (POSIX
// ERE).  Others use ECMAScript syntax, because the default seal::Regexp
// syntax is "Perl" (routed here since PCRE support is disabled), and
// ECMAScript is the closest match to it (\d, \s, [[:alpha:]], ...).

# include <regex>
# include <string>
# include <cstring>
# include <cstddef>

typedef std::ptrdiff_t regoff_t;

struct regex_t
{
    size_t	re_nsub;
    std::regex	*re_impl;
};

struct regmatch_t
{
    regoff_t	rm_so;
    regoff_t	rm_eo;
};

enum
{
    REG_EXTENDED = 1,
    REG_ICASE    = 2,
    REG_NOSUB    = 4,
    REG_NEWLINE  = 8
};

enum
{
    REG_NOTBOL = 1,
    REG_NOTEOL = 2
};

enum
{
    REG_NOMATCH = 1,
    REG_BADPAT  = 2
};

inline int
regcomp (regex_t *preg, const char *pattern, int cflags)
{
    using namespace std::regex_constants;
    syntax_option_type opts = (cflags & REG_EXTENDED) ? extended : ECMAScript;
    if (cflags & REG_ICASE)
	opts |= icase;
    if (cflags & REG_NOSUB)
	opts |= nosubs;
    if ((cflags & REG_NEWLINE) && ! (cflags & REG_EXTENDED))
	opts |= multiline;

    preg->re_nsub = 0;
    preg->re_impl = 0;
    try
    {
	preg->re_impl = new std::regex (pattern, opts);
	preg->re_nsub = preg->re_impl->mark_count ();
    }
    catch (const std::regex_error &)
    {
	return REG_BADPAT;
    }
    return 0;
}

inline int
regexec (const regex_t *preg, const char *string, size_t nmatch,
	 regmatch_t pmatch [], int eflags)
{
    using namespace std::regex_constants;
    if (! preg || ! preg->re_impl)
	return REG_BADPAT;

    match_flag_type mflags = match_default;
    if (eflags & REG_NOTBOL)
	mflags |= match_not_bol;
    if (eflags & REG_NOTEOL)
	mflags |= match_not_eol;

    std::cmatch m;
    if (! std::regex_search (string, string + std::strlen (string),
			     m, *preg->re_impl, mflags))
	return REG_NOMATCH;

    for (size_t i = 0; i < nmatch; ++i)
    {
	if (i < m.size () && m [i].matched)
	{
	    pmatch [i].rm_so = m [i].first - string;
	    pmatch [i].rm_eo = m [i].second - string;
	}
	else
	    pmatch [i].rm_so = pmatch [i].rm_eo = -1;
    }
    return 0;
}

inline void
regfree (regex_t *preg)
{
    if (preg)
    {
	delete preg->re_impl;
	preg->re_impl = 0;
	preg->re_nsub = 0;
    }
}

inline size_t
regerror (int errcode, const regex_t *, char *errbuf, size_t errbuf_size)
{
    const char *msg = (errcode == REG_NOMATCH ? "No match"
		       : errcode == REG_BADPAT ? "Invalid regular expression"
		       : "Unknown regular expression error");
    size_t len = std::strlen (msg) + 1;
    if (errbuf && errbuf_size)
    {
	size_t n = len < errbuf_size ? len : errbuf_size;
	std::memcpy (errbuf, msg, n - 1);
	errbuf [n - 1] = '\0';
    }
    return len;
}

#endif // SEAL_BASE_WINDOWS_REGEX_H
