/*#include "things.h"

void things::getServerConfig(int type, String arg[], memory memory, serverWifi serverWifi, timeClock timeClock){
    switch(type){
      //guardar apn
      case 0:{
        Serial.println("configurando APN");
        memory.setApn(arg[0]);
        memory.setApn_user(arg[1]);
        memory.setApn_pass(arg[2]);
      }break;
      //guardar ap
      case 1:{
        Serial.println("configurando AP");
        memory.setSsid_AP(arg[0]);
        memory.setPasswd_AP(arg[1]);
        serverWifi.AP_connect(memory);
      }break;
      //guardar sta
      case 2:{
        Serial.println("configurando STA");
        memory.setSsid_STA(arg[0]);
        memory.setPasswd_STA(arg[1]);
        //serverWifi.STA_connect(memory, timeClock);
      }break;
      //guardar la hora
      case 3:{
        Serial.println("configurando hora");
        int yeard=arg[0].substring(0,4).toInt();
        int mes=arg[0].substring(5,7).toInt()-1;
        int dia=arg[0].substring(8,10).toInt();
        int hora=arg[1].substring(0,2).toInt();
        int minu=arg[1].substring(3,6).toInt();
        timeClock.setTiempo(yeard, mes, dia, hora, minu);
      }break;
      //guardar el tiempo de espera
      case 4:{
        Serial.println("configurando time");
        memory.setTime_to_sleep(arg[0].toInt());
      }break;
    }
  memory.writeConfiguration("/config.txt");
}*/
