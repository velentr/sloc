#ifndef _CONFIG_H_
#define _CONFIG_H_


void parse_config(const char *file);

void addlanguage(void);
void assign_name(char *val);
void assign_startblk(char *val);
void assign_endblk(char *val);
void assign_starteol(char *val);
void add_extension(char *ext);

#endif /* end of include guard: _CONFIG_H_ */
