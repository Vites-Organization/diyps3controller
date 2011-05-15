/*
 * config_reader.h
 *
 *  Created on: 6 mai 2011
 *      Author: matlo
 */

#ifndef CONFIG_READER_H_
#define CONFIG_READER_H_

#include <libxml/xmlreader.h>

#define DEFAULT_RADIUS 50

void read_config_file(const char*);
int read_config_dir();

int GetIntProp(xmlNode*, char*, int*);
int GetUnsignedIntProp(xmlNode*, char*, unsigned int*);
int GetDoubleProp(xmlNode*, char*, double*);

#endif /* CONFIG_READER_H_ */