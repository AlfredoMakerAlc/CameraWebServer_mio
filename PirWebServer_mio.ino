#include <WebServer.h>

RTC_DATA_ATTR bool SensorActivado = true;
WebServer server(80);


/*
   Aqui esta definido todo el HTML y el CSS del servidor WEB con ESP32
*/

void AtenderClientes() {
  server.handleClient(); 
  if (SensorActivado)
  {
    Serial.println("Activando relé");
    pinMode(GPIO_NUM_13, OUTPUT); 
    digitalWrite(GPIO_NUM_13, HIGH);
  }
  else
  {
    Serial.println("Desactivando relé"); 
    pinMode(GPIO_NUM_13, INPUT);
    //digitalWrite(GPIO_NUM_13, LOW);  
  }
}

String SendHTML(bool estado) {
  // Cabecera de todas las paginas WEB
  String ptr = "<!DOCTYPE html> <html>\n";
  
  // <meta> viewport. Para que la pagina se vea correctamente en cualquier dispositivo
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Sensor PIR</title>\n";
  
/*
 * El estilo de la pagina WEB, tipo de letra, tamaño, colores, 
 * El estilos de los botones (las clases en CSS) y la definicion de como van a cambiar dependiendo de como
 * cambien los estado de los LEDs, color fondo etc
 */
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  /*
   * Encabezados de la pagina
   */
  ptr += "<h1>ESP32 Servidor WEB</h1>\n";
  ptr += "<h3>Usando Modo Estacion</h3>\n";
/*
 * Aqui esta la inteligencia del servidor WEB con ESP32, dependiento de los parametros de la funcion SendHTML
 * modificara la vista de la pagina WEB,  llamaran a las clases "button-on y button-off" que cambian como
 * se muestran los datos en la pagina WEB 
 */
  if (estado)
  {
    ptr += "<p>Sensor activado</p>\n";
  }
  else
  {
    ptr += "<p>Sensor desactivado</p>\n";
  }

    ptr += "</body>\n";
  ptr += "</html>\n";
  SensorActivado = estado;
  return ptr;
}

void handle_OnConnect() {
  
  Serial.println("Sensor activado" + SensorActivado); // 2
  server.send(200, "text/html", SendHTML(true)); // 3
}

void handle_activar() {
  
  Serial.println("Sensor activado"); // 2
  server.send(200, "text/html", SendHTML(true)); // 3
}

void handle_desactivar() {
  
  Serial.println("Sensor desactivado"); // 2
  server.send(200, "text/html", SendHTML(false)); // 3
}

void handle_NotFound() {
  server.send(404, "text/plain", "La pagina no existe");
}

void startMiServidor(){
  /*
 * Para procesar las solicitudes HTTP necesitamos definir el codigo que debe de ejecutar en
 * cada estado. Para ello utilizamos el metodo "on" de la libreria WebServer que hemos 
 * habilitador en la linea 13 de este codigo
 * 1 El primero se ejecuta cuando te conectas al Servidor WEB con ESP32 http://la_ip_del_esp32/
 * 2 Los 4 siguientes procesan los 2 estados que puede tener cada LED ON/OFF
 * 3 El ultimo gestiona los errores por ejemplo si pones http://la_ip_del_esp32/holaquetal
 * esta pagina no existe, por lo tanto actualizara la pagina WEB con un mensaje de error
 */
  server.on("/", handle_OnConnect); // 1
  server.on("/activar", handle_activar); // 2
  server.on("/desactivar", handle_desactivar); // 2
  server.onNotFound(handle_NotFound); // 3
/*
 * Arrancamos el Servicio WEB
 */
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}
