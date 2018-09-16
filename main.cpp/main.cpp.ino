#include <Ethernet.h>  //Importamos librería Ethernet

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//Ponemos la dirección MAC de la Ethernet Shield que está con una etiqueta debajo la placa
IPAddress ip(192,168,0,38); //Asingamos la IP al Arduino
IPAddress gateway(192,168,0, 1); // Puerta de enlace o Router
IPAddress subnet(255, 255, 255, 0); //Mascara de Sub Red (Subnet Mask)
EthernetServer server(80); //Creamos un servidor Web con el puerto 80 que es el puerto HTTP por defecto

const int ext_lights=2; //Pin de Arduino asignado a la ext_lights
const int hall_lights=3; //Pin de Arduino asignado al hall_lights
void processCheckbox(EthernetClient client,int pin);

void setup(){
 Ethernet.begin(mac, ip);
 server.begin();
 pinMode(ext_lights,OUTPUT);
 pinMode(hall_lights,OUTPUT);
}

void loop()
{
 EthernetClient client = server.available(); //Creamos un cliente Web
 //Cuando detecte un cliente a través de una petición HTTP
 if (client) {
   boolean currentLineIsBlank = true; //Una petición HTTP acaba con una línea en blanco
   String cadena=""; //Creamos una cadena de caracteres vacía
   while (client.connected()) {
     if (client.available()) {
       char c = client.read();//Leemos la petición HTTP carácter por carácter
       cadena.concat(c);//Unimos el String 'cadena' con la petición HTTP (c). De esta manera convertimos la petición HTTP a un String

        //Ya que hemos convertido la petición HTTP a una cadena de caracteres, ahora podremos buscar partes del texto.
        int posicion=cadena.indexOf("ext_lights="); //Guardamos la posición de la instancia "ext_lights=" a la variable 'posicion1'

         if(cadena.substring(posicion)=="ext_lights=1")//Si a la posición 'posicion1' hay "ext_lights=ON"
         {
           digitalWrite(ext_lights,HIGH);
         }
         if(cadena.substring(posicion)=="ext_lights=0")//Si a la posición 'posicion1' hay "ext_lights=OFF"
         {
           digitalWrite(ext_lights,LOW);
         }

        posicion =cadena.indexOf("hall_lights="); //Guardamos la posición de la instancia "hall_lights=" a la variable 'posicion2'

         if(cadena.substring(posicion)=="hall_lights=1")//Si a la posición 'posicion2' hay "hall_lights=ON"
         {
           digitalWrite(hall_lights,HIGH);;
         }
         if(cadena.substring(posicion)=="hall_lights=0")//Si a la posición 'posicion2' hay "hall_lights=OFF"
         {
           digitalWrite(hall_lights,LOW);
         }


       //Cuando reciba una línea en blanco, quiere decir que la petición HTTP ha acabado y el servidor Web está listo para enviar una respuesta
       if (c == 'n' && currentLineIsBlank) {

           // Enviamos al cliente una respuesta HTTP
           client.println("HTTP/1.1 200 OK");
           client.println("Content-Type: text/html");
           client.println();

           //Página web en formato HTML
           client.println("<html>");
           client.println("<head> <meta charset='utf-8'>");
           client.println("<link rel='stylesheet' href='https://gimenezpabl0.github.io/bootstrap.css' integrity='sha384-Smlep5jCw/wG7hdkwQ/Z5nLIefveQRIY9nfy6xoR1uRYBtpZgI6339F5dgvm/e9B' crossorigin='anonymous'>");
           client.println("<link rel='stylesheet' type='text/css' href='https://gimenezpabl0.github.io/index.css'/>");
           client.println("<link rel='stylesheet' type='text/css' href='https://gimenezpabl0.github.io/fontawesome-free-5.1.1-web/css/all.min.css'>");
           client.println("<link href='https://fonts.googleapis.com/css?family=Cabin|Lobster' rel='stylesheet'>");
           client.println("</head> <body>");
           client.println("<article> <header><p>Iluminación</p></header><div class=\"content\">");
           // client.println(day(start_time));
           client.println("<div class='accordion' id='accordionOne'>");
           processCheckbox(client,ext_lights);
           client.println("<p data-toggle='collapse' data-target='#collapseOne' aria-expanded='true' aria-controls='collapseOne'>LUCES EXTERIORES</p>");
           client.println("<div id='collapseOne' class='collapse' aria-labelledby='headingOne' data-parent='#accordionOne'>");
           client.println("<div class='card-body'><p>Tiempo:");
           //FUNCION PARA CALCULAR TIEMPO
           client.println("</p><p>Consumo de energía:");
           //FUNCION PARA CALCULAR EL CONSUMO DE NERGIA.
           client.println("</p></div></div></div>");
           client.println("<div class='accordion' id='accordionTwo'>");
           processCheckbox(client,hall_lights);
           client.println("<p data-toggle='collapse' data-target='#collapseTwo' aria-expanded='true' aria-controls='collapseTwo'>LUCES DEL LIVING</p>");
           client.println("<div id='collapseTwo' class='collapse' aria-labelledby='headingTwo' data-parent='#accordionTwo'>");
           client.println("<div class='card-body'><p>Tiempo:");
           //FUNCION PARA CALCULAR TIEMPO
           client.println("</p><p>Consumo de energía:");
           //FUNCION PARA CALCULAR EL CONSUMO DE NERGIA.
           client.println("</p></div></div></div>");
           //Creamos los botones. Para enviar parametros a través de HTML se utiliza el metodo URL encode. Los parámetros se envian a través del símbolo '?'
           client.println("</div></article></body>");
           client.println("</html>");
           break;
       }
       if (c == 'n') {
         currentLineIsBlank = true;
       }
       else if (c != 'r') {
         currentLineIsBlank = false;
       }
     }
   }
   //Dar tiempo al navegador para recibir los datos
   delay(1);
   client.stop();// Cierra la conexión
 }
}

void processCheckbox(EthernetClient client,int pin){
  switch(pin){
    case 2:
      if(digitalRead(pin)) client.println("<a href='ext_lights=0'><i class='fas fa-power-off fa-2x' style='color:red;'></i></a>");
      else client.println("<a href='ext_lights=1'><i class='fas fa-power-off fa-2x' style='color:green;'></i></a>");
      break;
    case 3:
      if(digitalRead(pin)) client.println("<a href='hall_lights=0'><i class='fas fa-power-off fa-2x' style='color:red;'></i></a>");
      else client.println("<a href='hall_lights=1'><i class='fas fa-power-off fa-2x' style='color:green;'></i></a>");
      break;
}
}
