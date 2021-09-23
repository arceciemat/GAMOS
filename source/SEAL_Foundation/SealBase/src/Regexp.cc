//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/Regexp.h"
#include "SealBase/RegexpMatch.h"
#include "SealBase/RegexpError.h"
#include "SealBase/DebugAids.h"
//#include "pcre.h"
#include <regex.h>
#include <cstring>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

// Base class for rep type
struct Regexp::Rep
{
    virtual ~Rep (void);

    virtual bool	good (void) const = 0;
    virtual const char	*errorstr (void) const = 0;
    virtual int		erroroff (void) const = 0;
    virtual void	clearerr (void) = 0;

    virtual bool	compile (const char *expr, int opts, int flags) = 0;
    virtual void	uncompile (void) = 0;
    virtual void	study (void);

    virtual int		search (const char *str, int offset, int limit,
				int flags, RegexpMatch *result) = 0;
};

// Convenience structures to hide the type details from clients.

struct Regexp::POSIXRep : public Rep
{
    POSIXRep (void);
    ~POSIXRep (void);

    virtual bool	good (void) const;
    virtual const char	*errorstr (void) const;
    virtual int		erroroff (void) const;
    virtual void	clearerr (void);

    virtual bool	compile (const char *expr, int opts, int flags);
    virtual void	uncompile (void);

    virtual int		search (const char *str, int offset, int limit,
				int flags, RegexpMatch *result);

    regex_t		*m_rx;
    std::string		m_errorstr;
    int			m_opts;
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const Regexp Regexp::rxwhite      ("\\s+");		// or: [ \n\t\r\v\f]+
const Regexp Regexp::rxint        ("-?\\d+");		// or: -?[0-9]+
const Regexp Regexp::rxdouble     ("-?(\\d+\\.\\d*|\\d+|\\.\\d+)([eE][-+]?\\d+)?");
// or: -?(([0-9]+\\.[0-9]*)|([0-9]+)|(\\.[0-9]+))([eE][---+]?[0-9]+)?
const Regexp Regexp::rxalpha      ("[[:alpha:]]+");	// or: [A-Za-z]+
const Regexp Regexp::rxlowercase  ("[[:lower:]]+");	// or: [a-z]+
const Regexp Regexp::rxuppercase  ("[[:upper:]]+");	// or: [A-Z]+
const Regexp Regexp::rxalphanum   ("[[:alpha:]\\d]+");	// or: [0-9A-Za-z]+
const Regexp Regexp::rxidentifier ("[A-Za-z_$][A-Za-z0-9_$]*");

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Regexp::Rep::~Rep (void)
{}

void
Regexp::Rep::study (void)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Regexp::POSIXRep::POSIXRep (void)
    : m_rx (0),
      m_opts (0)
{}

Regexp::POSIXRep::~POSIXRep (void)
{ uncompile (); }

bool
Regexp::POSIXRep::good (void) const
{ return m_rx; }

const char *
Regexp::POSIXRep::errorstr (void) const
{ return m_errorstr.c_str (); }

int
Regexp::POSIXRep::erroroff (void) const
{ return m_errorstr.empty () ? -1 : 0; }

void
Regexp::POSIXRep::clearerr (void)
{ m_errorstr.erase (m_errorstr.begin (), m_errorstr.end ()); }

bool
Regexp::POSIXRep::compile (const char *expr, int opts, int flags)
{
    ASSERT (expr);
    uncompile ();

    // Remember options, we need them in matching
    m_opts = opts;

    // Translate options.
    if (opts & IgnoreCase)
	flags |= REG_ICASE;
    if (opts & MultiLine)
	flags |= REG_NEWLINE;

    // FIXME: If AnchorAtStart, add ^

    // FIXME: Force C locale at this stage or at matching if option is
    // on?  Find out which stage they apply.
    m_rx = new regex_t;
    if (int rc = regcomp (m_rx, expr, flags))
    {
	m_errorstr.resize (regerror (rc, m_rx, 0, 0), '\0');
	regerror (rc, m_rx, &m_errorstr[0], m_errorstr.size ());
	regfree (m_rx);
	delete m_rx;
	m_rx = 0;
	return false;
    }

    return true;
}

void
Regexp::POSIXRep::uncompile (void)
{
    if (m_rx)
    {
	regfree (m_rx);
	delete m_rx;
	m_rx = 0;
    }

    m_errorstr.resize (0);
    m_opts = 0;
}

int
Regexp::POSIXRep::search (const char *subject, int offset, int limit,
			  int flags, RegexpMatch *result)
{
    // FIXME: m_opts & DollarAtEndOnly
    // FIXME: m_opts & AnchorAtStart
    // FIXME: m_opts & NoMatchData
    // FIXME: MatchAnchored

    ASSERT (offset >= 0);
    ASSERT (offset <= limit);

    // Determine flags to pass to pcre
    int rxflags = 0;
    if (flags & MatchNotBOL)
	rxflags |= REG_NOTBOL;

    if (flags & MatchOffsetBOL)
    {
	subject += offset;
	limit -= offset;
	offset = 0;
	rxflags &= ~REG_NOTBOL;
    }

    if (flags & MatchNotEOL)
	rxflags |= REG_NOTEOL;

    // If the subject is longer than limit, make a local copy of an
    // appropriate length.  Subject might contain nulls earlier than
    // limit, in which case posix will stop searching at that point;
    // we don't bother checking for that.
    std::vector<char> subjcopy;
    if (subject [limit-1] != 0)
    {
	subjcopy.resize (limit);
	memcpy (&subjcopy [0], subject, limit-1);
	subjcopy [limit-1] = 0;

	subject = &subjcopy [0];
    }

    // Make space for the result.  We do this locally to avoid mt
    // headaches -- we can't keep the working space in the regexp, and
    // we can't put this in the result because we might not have one
    // and anyway the type cannot be exposed to outside.
    //
    // We always keep track of at least the full string even if no
    // results were requested so that we can compute the right return
    // value (== first match location).
    std::vector<regmatch_t>	ovec;
    int				novec = m_rx->re_nsub + 1;
    int				first = -1;
    int				rc = 0;

    ovec.resize (novec);
    while (offset < limit)
    {
	// Adjust beginning-of-line flag if necessary.  REG_NOTBOL
	// only matters at the beginning of the subject; in multi-line
	// mode ^ matches after new-lines despite REG_NOTBOL.  So in
	// multi-line make sure the flag reflects whether or not the
	// offset is just after beginning of string or new-line.  In
	// non-multi-line mode turn the flag on simply if we are past
	// the beginning of the subject.
	//
	// We have to do this always before matching: POSIX regexps
	// lack a match offset concept,so we have to simulate it by
	// twiddling the flags.
        if (offset > 0) {
	  if ((m_opts & MultiLine) && subject [offset-1] == '\n') {
	    rxflags &= ~REG_NOTBOL;
	  } else {
	    rxflags |= REG_NOTBOL;
	  }
	}

      // Try matching; in case of failure bail out.
	if ((rc = regexec (m_rx, subject + offset, novec, &ovec[0], rxflags)))
	    break;

	ASSERT (ovec [0].rm_so >= 0);
	ASSERT (ovec [0].rm_eo >= 0);
	ASSERT (ovec [0].rm_eo <= limit);

	// If we matched an unnacceptable empty match, ignore it and
	// loop around with an increased offset.  This works because
	// POSIX regexps are always greedy: if the regexp matched null
	// string here, it couldn't have matched a non-empty string,
	// and OTOH it will match anywhere, so it must have matched
	// immediately at the current offset.  So if empty matches are
	// not acceptable, move on, we don't need to worry about
	// trying to match a non-null string at the same position.
	if (! ((flags & MatchNoEmpty)
	       && ovec[0].rm_so == 0
	       && ovec[0].rm_eo == 0))
	{
	    // Merge match results.  For each match we add complete
	    // result set.  It's up the client to inspect the right
	    // ones.  Note that since POSIX doesn't know about offset,
	    // we need to fold it in artificially.
	    if (result)
	    {
		result->setCaptures (m_rx->re_nsub);
		result->addMatchSet ();
		for (size_t i = 0; i <= m_rx->re_nsub; ++i)
		    if (ovec [i].rm_so == -1)
			result->addMatchPos (-1, -1);
		    else
			result->addMatchPos (ovec [i].rm_so + offset,
					     ovec [i].rm_eo + offset);
	    }

	    // Update first match position.
	    if (first == -1)
		first = ovec [0].rm_so + offset;

	    ASSERT (first >= 0);

	    // Bail out now if we are not intrested in further matches.
	    if (! (flags & MatchAll))
		break;
	}

	// Adjust search position past the end of the current match.
	offset += ovec [0].rm_eo;
    }

    // Throw an exception in case of a fatal error.
    if (rc < 0 && rc != REG_NOMATCH)
	// FIXME: put syntax right!
	throw RegexpError ("regexec()", POSIXBasic, rc);

    // Return the location of the first match.
    return first;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
Regexp::Regexp (void)
    : m_options (Perl),
      m_rep (0)
{}

Regexp::Regexp (const std::string &pat,
		unsigned options /* = 0 */,
		Syntax s /* = Perl */)
    : m_pattern (pat),
      m_options (options | s),
      m_rep (0)
{ compile (); }

Regexp::Regexp (const Regexp &x)
    : m_pattern (x.m_pattern),
      m_options (x.m_options),
      m_rep (0)
{ compile (); }

Regexp &
Regexp::operator= (const Regexp &x)
{
    if (this != &x)
    {
	// FIXME: Copy compiled regexp data?  This is difficult, at
	// least pcre data is interdependent (pcre refers to locale)
	// and we don't know the size anyway.
	m_pattern = x.m_pattern;
	m_options = x.m_options;
	compile (true);
    }
    return *this;
}

Regexp &
Regexp::operator= (const std::string &x)
{
    m_pattern = x;
    compile (true);
    return *this;
}

Regexp::~Regexp (void)
{ uncompile (); delete m_rep; }

//////////////////////////////////////////////////////////////////////
bool
Regexp::operator== (const Regexp &x) const
{ return m_pattern == x.m_pattern && m_options == x.m_options; }

bool
Regexp::operator!= (const Regexp &x) const
{ return ! (*this == x); }

//////////////////////////////////////////////////////////////////////
std::string
Regexp::escape (const std::string &text)
{
    std::string rewrite;
    rewrite.reserve (text.size ());
    for (size_t i = 0; i < text.size (); ++i)
	switch (text [i])
	{
	case '\0':
	    rewrite += "\\000";
	    break;

	case '\\':	case '^':	case '$':	case '.':
	case '[':	case '|':	case '(':	case ')':
	case '?':	case '*':	case '+':	case '{':
	case '#':
	    rewrite += '\\';
	    // fall through

	default:
	    rewrite += text [i];
	}

    return rewrite;
}

//////////////////////////////////////////////////////////////////////
Regexp::Syntax
Regexp::syntax (void) const
{ return Syntax (m_options & SyntaxMask); }

bool
Regexp::empty (void) const
{ return m_pattern.empty (); }

bool
Regexp::valid (void) const
{ ASSERT (m_rep); return m_rep->good (); }

int
Regexp::errorOffset (void) const
{ return m_rep ? m_rep->erroroff () : -1; }

const char *
Regexp::errorMessage (void) const
{ return m_rep ? m_rep->errorstr () : ""; }

void
Regexp::clearError (void)
{ if (m_rep) m_rep->clearerr (); }

//////////////////////////////////////////////////////////////////////
unsigned
Regexp::options (void) const
{ return m_options & ~SyntaxMask; }

void
Regexp::setOptions (unsigned value)
{ m_options = value | syntax (); compile (true); } // FIXME: check value

void
Regexp::enableOptions (unsigned mask)
{ m_options |= mask; compile (true); } // FIXME: check mask

void
Regexp::disableOptions (unsigned mask)
{ m_options &= ~mask; compile (true); } // FIXME: check mask

//////////////////////////////////////////////////////////////////////
std::string
Regexp::pattern (void) const
{ return m_pattern; }

void
Regexp::setPattern (const std::string &pat)
{ this->operator= (pat); compile (true); }

//////////////////////////////////////////////////////////////////////
bool
Regexp::compile (bool force /* = false */)
{
    // If forcing, master reset rep.
    if (force)
	uncompile ();

    // If current compiled pattern is ok, reuse it.
    if (m_rep && m_rep->good ())
	return true;

    const char	*pattern = m_pattern.c_str ();
    std::string rewrite;
    int		flags = 0;
    Syntax	syn = syntax ();

    if (syn == Wildcard)
    {
	// Rewrite the glob pattern for PCRE.
	rewrite.reserve (m_pattern.size () * 2 + 4);
	for (const char *s = m_pattern.c_str (); *s; ++s)
	    switch (*s) {
	    case '[':
		// Translate character classes.  Glob classes cannot
		// be empty so we need some extra care.  In general
		// class handling is close enough with PCRE and glob
		// patterns so we don't actually have to do much here.
		if (*++s == '!')
		{
		    // "[!...]" => "[^...]".  If the next character is
		    // ']', it is part of the class and we want add it
		    // here to avoid scan code below stopping at it.
		    rewrite += "[^";
		    if (*++s == ']')
			++s, rewrite += ']';
		}
		else if (*s == '^')
		    // Found "[^".  Insert a '\0' character (which
		    // cannot happen in a filename) into the character
		    // class so that '^' is not the first character
		    // after '[', and thus special in the regexp.
		    // PCRE is ok with null characters in patterns
		    // provided they are quoted (the pattern is taken
		    // as null-terminated string so we can't add a
		    // real null here).
		    ++s, rewrite += "[\\000^";
		else if (*s == '[')
		    // Found "[]".  The closing bracket should be
		    // taken as a first character of the class as
		    // already mentioned above.
		    ++s, rewrite += "[]";
		else
		    rewrite += '[';

		// Copy everything up to and including the next ']'.
		while (*s && *s != ']')
		    rewrite += *s;

		// Back out one character, loop above will increment it
		--s;
		break;

		// Handle all other special characters in glob
		// wildcards.  Note in particular that '\' is not
		// special in wildcards, only in shell!

	    case '.':	rewrite += "\\.";	break;
	    case '*':	rewrite += "[^\\000]*";	break;
	    case '+':	rewrite += "\\+";	break;
	    case '^':	rewrite += "\\^";	break;
	    case '$':	rewrite += "\\$";	break;
	    case '\\':	rewrite += "\\\\";	break;
	    case '?':	rewrite += "[^\\000]";	break;
	    default:	rewrite += *s;		break;
	    }

	// Wildcards must always match the full string.
	rewrite = "\\A" + rewrite + "\\z";

	// Refer the pattern to our local rewrite.
	pattern = rewrite.c_str ();

	// Fall through to standard perl regex.
	syn = Perl;
    }
    else if (syn == POSIXExtended)
    {
	// Extended POSIX regexps are handled just like the basic
	// ones, we just need to set one extra bit in the flags.
	flags = REG_EXTENDED;

	// Fall through to standard posix regex.
	syn = POSIXBasic;
    }

    // Make sure we have a rep
    //    if (syn == Perl && ! m_rep)
    //	m_rep = new PCRERep;
    // else if (syn == POSIXBasic && ! m_rep)
	m_rep = new POSIXRep;

    ASSERT (syn == Perl || syn == POSIXBasic);
    //    ASSERT (syn != Perl || dynamic_cast<PCRERep *> (m_rep));
    ASSERT (syn != POSIXBasic || dynamic_cast<POSIXRep *> (m_rep));

    // Compile it.
    return m_rep->compile (pattern, m_options, flags);
}

void
Regexp::uncompile (void)
{ if (m_rep) m_rep->uncompile (); }

void
Regexp::study (void)
{ if ((m_rep && m_rep->good ()) || compile ()) m_rep->study (); }

//////////////////////////////////////////////////////////////////////
// FIXME: set match length to the partial accepted length
// (e.g. pat "blue", s "blutak" -> len = 3).
bool
Regexp::exactMatch (const std::string &s) const
{
    RegexpMatch m;
    return search (s, 0, 0, &m) == 0 && size_t (m.matchEnd ()) == s.size ();
}

bool
Regexp::match (const std::string &s, int offset /* = 0 */,
	       int flags /* = 0 */, RegexpMatch *result /* = 0 */) const
{
    // An invalid pattern always fails to match.  Don't touch
    // the result, it is assumed to be only added to.
    if (! m_rep || ! m_rep->good ())
	return -1;

    // Adjust offset
    if (offset < 0)
	offset += s.size ();

    if (offset < 0)
	offset = 0;

    if ((size_t) offset > s.size ())
	offset = s.size ();

    // Clear last match
    if (result && ! (flags & MatchContinue))
	result->reset ();

    // FIXME: This is efficient for pcre but not for posix; the latter
    // provides no means to stop search() from moving in the string
    // instead of just testing at `offset'.
    return m_rep->search (s.c_str (), offset, s.size (),
			  (flags & ~MatchAll) | MatchAnchored,
			  result)
	== offset;
}

// Returns offset of first match or -1.
int
Regexp::search (const std::string &s, int offset /* = 0 */,
		int flags /* = 0 */, RegexpMatch *result /* = 0 */) const
{
    // An invalid pattern always fails to match.  Don't touch
    // result, it is assumed to be only added to.
    if (! m_rep || ! m_rep->good ())
	return -1;

    // Adjust offset
    if (offset < 0)
	offset += s.size ();

    if (offset < 0)
	offset = 0;

    if ((size_t) offset > s.size ())
	offset = s.size ();

    // Clear last match
    if (result && ! (flags & MatchContinue))
	result->reset ();

    // Match
    return m_rep->search (s.c_str (), offset, s.size (), flags, result);
}

int
Regexp::rsearch (const std::string &s, int offset /* = -1 */,
		 int flags /* = 0 */, RegexpMatch *result /* = 0 */) const
{
    // FIXME: If we are going to look for all matches, might as well
    // search forward for all matches and simply reverse the result.

    // An invalid pattern always fails to match.  Don't touch
    // the result, it is assumed to be only added to.
    if (! m_rep || ! m_rep->good ())
	return -1;

    // Adjust offset
    if (offset < 0)
	offset += s.size ();

    if (offset < 0)
	return -1;

    if ((size_t) offset > s.size ())
	offset = s.size ();
    
    // Clear last match
    if (result && ! (flags & MatchContinue))
	result->reset ();

    // Look for matches; if we find one, merge results.  Only accept a
    // match at this position; ignore the matches elsewhere.
    int		index = -1;
    RegexpMatch m;

    for ( ; offset >= 0; --offset)
	if (match (s, offset, flags & ~(MatchContinue|MatchAll), &m))
	{
	    if (index == -1)
		index = offset;

	    if (result)
		result->addMatches (m);

	    if (! (flags & MatchAll))
		break;
	}

    return index;
}

} // namespace seal
