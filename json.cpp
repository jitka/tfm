#include "json.h"

void skipWhiteSpace(QString &in){
	//ze vstupu ureze vse co neni mezera
	while ( !in.isEmpty() && in[0].isSpace())
		in.remove(0,1);
}

bool parseInt(QString &in, int &number) {
	number = 0;
	int sgn = 1;
	if (in[0] == '-'){
		sgn = -1;
		in.remove(0,1);
	}

	while ( !in.isEmpty() ){

		if (in[0].isDigit()){
			number = number*10 + in[0].digitValue();
			in.remove(0,1);
		} else {
			number *= sgn;
			return true;
		}
	}
	return true;
}

bool parseString(QString &in, QString &s){
	s = "";
	in.remove(0,1); // '"'

	while (true) {
		if ( in.isEmpty() ){
			return false;
		} else if ( in[0] == '"' ){
			in.remove(0,1);
			return true;
		} else if ( in[0] == '\\'){
			in.remove(0,1);
			if ( in.isEmpty() )
				return false;
			switch ( in[0].toAscii() ){
			case '"': s += "\""; break;
			case '/': s += "/"; break;
			case 'b': s += "\b"; break;
			case 'f': s += "\f"; break;
			case 'n': s += "\n"; break;
			case 'r': s += "\r"; break;
			case 't': s += "\t"; break;
			//case 'u': std::cerr << "ERR neumim\n"; break;
			default: s += "\\"; s += in[0]; break;
			} 
			in.remove(0,1);
		} else {
			s += in[0];
			in.remove(0,1);
		}

	}
}

bool parseVector(QString &in, QVector<Json> &v){

	in.remove(0,1); // '['

	skipWhiteSpace(in);
	if ( in[0] == ']' ){
		in.remove(0,1);
		return true;
	}

	while (true){
		skipWhiteSpace(in);

		Json p(in);
		if (p.type == ERROR)
			return false;

		skipWhiteSpace(in);

		//co je po skonceni jedne polozky
		if ( in.isEmpty() ){
			return false;
		} else if ( in[0] == ']' ){
			in.remove(0,1);
			v.push_back(p);
			return true;
		} else if ( in[0] == ',' ){
			in.remove(0,1);
			v.push_back(p);
		} else {
			return false;
		}
	}	
}


bool parseMap(QString &in, QMap<QString,Json> &m){

	in.remove(0,1); // '{'

	skipWhiteSpace(in);
	if ( in[0] == '}' ){
		in.remove(0,1);
		return true;
	}

	while (true){

		skipWhiteSpace(in);

		// " neco "
		QString str;
		if (!parseString(in,str))
			return false;

		skipWhiteSpace(in);

		// :
		if ( in.isEmpty() ){
			return false;
		} else if ( in[0] == ':' ){
			in.remove(0,1);
		} else {
			return false;
		}

		skipWhiteSpace(in);

		// polozka
		Json p(in);
		if (p.type == ERROR)
			return false;

		skipWhiteSpace(in);

		// ,
		if ( in.isEmpty() ){
			return false;
		} else if ( in[0] == '}' ){
			in.remove(0,1);
			m.insert(str,p);
			return true;
		} else if ( in[0] == ',' ){
			in.remove(0,1);
			m.insert(str,p);
		} else {
			return false;
		}

	}	

}

bool readRest(QString &in, const QString &rest) {
	QString r = rest;
	while (true) {
		if ( r.isEmpty() ){
			return true;
		} else if ( in.isEmpty() ){
			return false;
		} else if ( in[0] != r[0] ){
			return false;
		} else {
			in.remove(0,1);
			r.remove(0,1);
		}

	}
}

Json::Json(){
	type = MY_NULL;
}

Json::Json(QString &in){
	
	skipWhiteSpace(in);
	
	if ( in.isEmpty() ){
		type = ERROR;
		return;
	}
	
	switch (in[0].toAscii()){
	case '-': 	case '0':	case '1':
	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	
	case '8':	case '9':
		{
			int number;
				if ( parseInt(in, number) ){
					type = INT;
					value.number = number;
				} else {
					type = ERROR;
				}
			break;
		}
	case '[':
		{
			QVector<Json>* v = new QVector<Json>();
				if ( parseVector(in, *v) ){
					type = VECTOR;
					value.vector = v;
				} else {
					type = ERROR;
				}
			break;
		}
	case '{':
		{
			QMap<QString,Json>* m = new QMap<QString,Json>();
				if ( parseMap(in, *m) ){
					type = MAP;
					value.map = m;
				} else {
					type = ERROR;
				}
			break;
		}

	case '"':	
		{
			QString* str = new QString();
				if ( parseString(in, *str) ){
					type = STRING;
					value.string = str;
				} else {
					type = ERROR;
				}
			break;
		}
	case 't':
		if ( readRest(in,"true") ){
			type = MY_TRUE;
		} else {
			type = ERROR;
		}
		break;
	case 'f':
		if ( readRest(in,"false") ){
			type = MY_FALSE;
		} else {
			type = ERROR;
		}
		break;
	case 'n':
		if ( readRest(in,"null") ){
			type = MY_NULL;
		} else {
			type = ERROR;
		}
		break;
	default:
		type = ERROR;
		break;
	}
}

////////////////////////////////////////////////////////////////////

Json::Json(const Json &json){
	type = json.type;
	switch (json.type) {
		case INT:
			value.number = json.value.number;
			break;
		case STRING:
			value.string = new QString(*json.value.string);
			break;
		case MAP:
			value.map = new QMap<QString,Json>(*json.value.map);
			break;
		case VECTOR:
			value.vector = new QVector<Json>(*json.value.vector);
			break;
		default:
			break;
	}
	
}

Json::~Json(){

	switch (type) {
		case STRING:
			delete value.string;
			break;
		case MAP:
			delete value.map;
			break;
		case VECTOR:
			delete value.vector;
			break;
		default:
			break;
	}
	

}

////////////////////////////////////////////////////////////////////

/*
void writeString(QTextStream &out, const QString &str){
//	qDebug() << str;
	out << "\"";
	for (int i = 0; i < str.length(); i++){
		switch  (str[i].toAscii()){
		case '\"': out << "\\\""; break;
		case '/': out << "\\/"; break;
		case '\b': out << "\\b"; break;
		case '\f': out << "\\f"; break;
		case '\n': out << "\\n"; break;
		case '\r': out << "\\r"; break;
		case '\t': out << "\\t"; break;
		default: out << str[i]; break;
		} 
	}
	out << "\"";
}

void writeVector(QTextStream &out, const QVector<Json> &vector){
	out << "[ ";
	bool begin = true;
	for (int i = 0; i < vector.size(); i++){
		if (!begin)
			out << ", ";
		begin = false;
		vector[i].write(out);

	}
	out << " ]";
}


void writeMap(QTextStream &out, const QMap<QString,Json> &map){
	out << "{ ";
	QMapIterator<QString,Json> it(map);

	bool begin = true;
	while (it.hasNext()) {
		it.next();
		if (!begin)
			out << ", ";
		begin = false;

		writeString(out,it.key());
		out << " : ";
		it.value().write(out);
	}
	out << " }";

}

void Json::write(QTextStream &out) const{
	switch (type) {
	case INT:
		out << value.number;
		break;
	case STRING:
		writeString(out, *value.string);
		break;
	case MAP:
		writeMap(out, *value.map);
		break;
	case VECTOR:
		writeVector(out, *value.vector);
		break;
	case MY_TRUE: 
		out << "true";
		break;
	case MY_FALSE:
		out << "false";
		break;
	case MY_NULL:
		out << "null";
		break;
	case ERROR:
		out << "error";
		break;
	}
}
*/
