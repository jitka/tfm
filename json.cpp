#include "json.h"

int parsujInt(std::istream &vstup){
	int cislo = 0;
	int znamenko = 1;
	char c;
	vstup.get(c);
	if (c == '-') {
		znamenko = -1;
		vstup.get(c);
	}	
	cislo = c-'0';
	while (vstup.get(c)){
		switch (c){
		case '0':	case '1':
		case '2':	case '3':	case '4':
		case '5':	case '6':	case '7':	
		case '8':	case '9':
			cislo = cislo*10 + (int) (c-'0');
			break;
		default:
			vstup.unget();
			return cislo*znamenko;
		}

	}
}

std::string parsujString(std::istream &vstup){
	char c;
	std::string s = ""; //pridavat pomoci +
	while (vstup.get(c)){
		if (c == '"'){
	//		std::cout << s << "?\n";
			return std::string(s);
		}
		if (c == '\\'){
			vstup.get(c);
			switch (c){
			case '"': s += "\""; break;
			case '/': s += "/"; break;
			case 'b': s += "\b"; break;
			case 'f': s += "\f"; break;
			case 'n': s += "\n"; break;
			case 'r': s += "\r"; break;
			case 't': s += "\t"; break;
			case 'u': std::cerr << "ERR neumim\n"; break;
			default: s += "\\"; s += c; break;
			} 
		} else {
			s += c;
		}
	}
}

void nemezera(std::istream &vstup, char &c){
		do { vstup.get(c); } //mezery do zacatku vcente
		while (isspace(c));
}

std::vector<polozka>* parsujVector(std::istream &vstup){
	std::vector<polozka> *v = new std::vector<polozka>();
	char c;
	while (true){
		nemezera(vstup,c);

		if (c == ']'){
			return v;
		}
	
		vstup.unget();
		polozka p = parsuj(vstup);
		v->push_back(p);

		nemezera(vstup,c);
		
		if (c == ']')
			return v;
	}
}

std::map<std::string,polozka>* parsujMap(std::istream &vstup){
	std::map<std::string,polozka> *m = new std::map<std::string,polozka>();
	char c;
	while (true){
		nemezera(vstup,c);

		if (c == '}')
			return m;
	
		std::string str =  parsujString(vstup);

		nemezera(vstup,c);
		nemezera(vstup,c);

		vstup.unget();
		polozka p = parsuj(vstup);

		(*m)[str] = p;

		nemezera(vstup,c);
		
		if (c == '}')
			return m;
	}

}


void docti(std::istream &vstup, std::string str){
	char c;
	for (int i = 0; i < str.length(); i++){
		vstup.get(c);
	}
}


polozka parsuj(std::istream &vstup){
	polozka p;
	char c;

	nemezera(vstup,c);
	
	switch (c){
	case '-': 	case '0':	case '1':
	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	
	case '8':	case '9':
		//int x = parsujInt(vstup, c) << "\n";
		vstup.unget();
		p.typ = INT;
		p.hodnota.Int = parsujInt(vstup);
		break;
	case '[':
		p.typ = VECTOR;
		p.hodnota.Vector = parsujVector(vstup);
		break;
	case '{':	
		p.typ = MAP;
		p.hodnota.Map = parsujMap(vstup);
		break;
	case '"':	
		p.typ = STRING;
		p.hodnota.String = new std::string(parsujString(vstup));
		break;
	case 't':
		docti(vstup,"rue");
		p.typ = MY_TRUE;
		break;
	case 'f':
		docti(vstup,"alse");
		p.typ = MY_FALSE;
		break;
	case 'n':
		docti(vstup,"ull");
		p.typ = MY_NULL;
		break;
	default:
		std::cerr << c << " hui\n";
	}
	return p;
}

//--------------------------------------------------------------------------

void vypisString(std::ostream &vystup, std::string str){
	vystup << "\"";
	for (int i = 0; i < str.length(); i++){
		switch  (str[i]){
		case '\"': vystup << "\\\""; break;
		case '/': vystup << "\\/"; break;
		case '\b': vystup << "\\b"; break;
		case '\f': vystup << "\\f"; break;
		case '\n': vystup << "\\n"; break;
		case '\r': vystup << "\\r"; break;
		case '\t': vystup << "\\t"; break;
		default: vystup << str[i]; break;
		} 
	}
	vystup << "\"";
}

void vypisVector(std::ostream &vystup, std::vector<polozka> *vector){
	vystup << "[ ";
	bool zacatek = true;
	for (int i = 0; i < vector->size(); i++){
		if (!zacatek)
			vystup << ", ";
		zacatek = false;
		vypis(vystup,(*vector)[i]);

	}
	vystup << " ]";
}

void vypisDvojice(std::ostream &vystup, std::string str, polozka polozka){
	vypisString(vystup,str);
	vystup << " : ";
	vypis(vystup,polozka);
}

void vypisMap(std::ostream &vystup, std::map<std::string,polozka> *map){
	vystup << "{ ";
	std::map<std::string,polozka>::iterator it;

	bool zacatek = true;
	for (it = map->begin(); it != map->end(); ++it){
		if (!zacatek)
			vystup << ", ";
		zacatek = false;
		vypisDvojice(vystup,(*it).first,(*it).second);
	}
	vystup << " }";

}

void vypis(std::ostream &vystup, polozka p){
	switch (p.typ) {
	case INT:
		vystup << p.hodnota.Int;
		break;
	case STRING:
		vypisString(vystup, *p.hodnota.String);
		break;
	case MAP:
		vypisMap(vystup, p.hodnota.Map);
		break;
	case VECTOR:
		vypisVector(vystup, p.hodnota.Vector);
		break;
	case MY_TRUE: 
		vystup << "true";
		break;
	case MY_FALSE:
		vystup << "false";
		break;
	case MY_NULL:
		vystup << "null";
		break;
	case ERROR:
		vystup << "null";
		break;
	}
}

//-----------------------------------------------

void smaz(polozka p){
	//TODO testovat
	//ostatni se mazou s koncem polozky
	switch (p.typ) {
	case STRING:
		delete p.hodnota.String;
		break;
	case MAP:
		{
			std::map<std::string,polozka> *map = p.hodnota.Map;
			std::map<std::string,polozka>::iterator it;
			for (it = map->begin(); it != map->end(); ++it){
				smaz((*it).second);
			}
			delete p.hodnota.Map;
			break;
		}
	case VECTOR:
		{
			std::vector<polozka> *vector = p.hodnota.Vector;
			std::vector<polozka>::iterator it;
			for (it = vector->begin(); it != vector->end(); ++it){
				smaz(*it);
			}
			delete p.hodnota.Map;
			break;
		}

	}
}


