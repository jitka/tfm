#ifndef JSON_H_
#define JSON_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

typedef enum {INT,STRING,MAP,VECTOR,MY_TRUE,MY_FALSE,MY_NULL,ERROR} typ_t;

typedef struct polozka{ //kdyz tak prejmenovat
	typ_t typ;
	union { int Int;
		std::string *String;
		std::map<std::string,polozka> *Map;
		std::vector<polozka> *Vector;
	} hodnota;
} polozka;

polozka parsuj(std::istream &vstup);

void vypis(std::ostream &vystup, polozka p);
void smaz(polozka p); //pokud je pole nebo objekt smaze syny
void odeber(polozka &kam);
void odeber(polozka &kam, std::string str);

#endif
