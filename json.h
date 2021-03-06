#ifndef JSON_H_
#define JSON_H_

#include <QString>
#include <QMap>
#include <QVector>

typedef enum {INT,STRING,MAP,VECTOR,MY_TRUE,MY_FALSE,MY_NULL,ERROR} type_t;

class Json { 
	public:
		type_t type;
		union { int number;
			QString* string;
			QMap<QString,Json>* map;
			QVector<Json>* vector;
		} value;

		Json(QString &in);
		Json();
		Json(const Json &json);
		~Json();

//		void write(QTextStream &out) const;

};

#endif
