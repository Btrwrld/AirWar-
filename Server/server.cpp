
#include "server.h"
#include <string>
void server::init() {int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] ,
            max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[1025];  //buffer de datos de 1K

    //set de socket descriptors
    fd_set readfds;

    //Mensaje que se envia al conectarse
    char *message = "Conexion establecida" "\n";

    //Inicializa los clientes
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //crea un socket maestro
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //hace al socket maestro aceptar multiples conexiones
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //Variables de socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.0.123");//Definimos la IP
    address.sin_port = htons( PORT );

    //se hace un bind a la ip, puerto 8080
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Escuchando en el puerto %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //Aceptamos la conexion
    addrlen = sizeof(address);
    puts("Esperando conexiones ...");

    while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //Añade sockets hijos
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //Esperamos a que haya actividad en algun socket
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        //Si algo le ocurre al socket maestro, es una conexion
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //se informa sobre el numero de socket al usuario
            printf("Nueva conexion , socket fd %d , ip : %s , puerto : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs
                    (address.sin_port));

            //se envia un nuevo mensaje de conexion
            if( send(new_socket, message, strlen(message), 0) != strlen(message) )
            {
                perror("send");
            }

            puts("Mensaje enviado con exito");

            //añade un nuevo socket al array
            for (i = 0; i < max_clients; i++)
            {
                
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Añadido a la lista de sockets como %d\n" , i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Leemos el mensaje y verificamos si fue para desconectar
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Si alguien se desconecta, se notifica
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);
                    printf("Host desconectado , ip %s , port %d \n" ,
                           inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Cerramos el socket y le asignamos valor 0, para volver a utilizar
                    close( sd );
                    client_socket[i] = 0;
                }
                //Se aplicaran los metodos de acuerdo al protocolo
                else
                {


                    buffer[valread] = '\0';
                    //metodo de envio, por sockets
                    send(sd, buffer, strlen(buffer), 0);


                }
            }
        }
    }
}
