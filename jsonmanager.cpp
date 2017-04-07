

#include "jsonmanager.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace rapidjson;

void jsonmanager::read(char* json){
    Document d;
    d.Parse(json);
    Value& type=d["get"];
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

char* jsonmanager::write(){

}
