

#include "jsonmanager.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace rapidjson;

void readJson(char* json){
    Document d;
    d.Parse(json);
    Value& type=d["type"];
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

const char* write(int pVidas, int pPuntuacion){
    Document d;
    Document::AllocatorType& alloc = d.GetAllocator();

    std::string mytext = "this is my text";

    d.SetObject();

    d.AddMember("type", "update", alloc);
    d.AddMember("score", pPuntuacion, alloc);
    d.AddMember("lives", pVidas, alloc);


    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);

    return buffer.GetString();

}
