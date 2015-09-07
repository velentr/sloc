#ifndef _SLOC_H_
#define _SLOC_H_


#define SET_VERSION(major, minor, bugfix) \
    const int MAJOR_VERSION    = major;   \
    const int MINOR_VERSION    = minor;   \
    const int BUGFIX_VERSION   = bugfix;  \
    const char *VERSION_STRING = #major "." #minor "." #bugfix;

SET_VERSION(2,0,0);

#endif /* _SLOC_H_ */
