/**
 * count.h
 *
 * Count lines of code.
 *
 * Contains function prototypes and datatypes for acutally counting code,
 * comments, files, etc. using a file-tree walk. The counts will be stored in
 * the langspec vector (see langspec.{c,h}).
 *
 * Revision History:
 *       4 Sep 2015     Brian Kubisiak      Initial revision.
 */
#ifndef _COUNT_H_
#define _COUNT_H_


void count_from(const char *root);


#endif /* end of include guard: _COUNT_H_ */
