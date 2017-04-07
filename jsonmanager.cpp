

#include "jsonmanager.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace rapidjson;

void jsonmanager::read(char* json){
    Document d; //Crea un rapidjson document, que es la manera en la que se va a tratar el json
    d.Parse(json); //Hace un parse del json a document
    Value& type=d["type"]; //Obtenemos el valor del type
    //Metodos del juego
    if(type.GetString()=="shoot"){
        //Dispara
    }else if(type.GetString()=="izq"){
        //Mov. izquierda
    }else if(type.GetString()=="der"){
        //Mov. derecha
    }else if(type.GetString()=="arr"){
        //Mov. arriba
    }else if(type.GetString()=="aba"){
        //Mov. abajo
    }else{
        printf("JSON no valido");
    }

}
