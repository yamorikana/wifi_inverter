#ifdef ESP8266

  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);

#else // それ以外･･･

  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
  WebServer server( 80 );

#endif


#define WIFI_SSID "INVERTER(open_to_http192.168.10.1)"
#define WIFI_PWD ""

#define HTML_HEADER "<!doctype html>"\
  "<html><head><meta charset=\"UTF-8\"/>"\
  "<meta name=\"viewport\" content=\"width=device-width\"/>"\
  "</head><body>"
#define HTML_FOOTER "</body></html>"
#define HTML_GOTOTOP "<h3>※１秒後トップページに自動的に移動します。</h3><meta http-equiv=\"Refresh\" content=\"1; url=http://192.168.10.1/\">"

#define GPIO_FON 5
#define GPIO_RON 4
#define GPIO_STOP 15

IPAddress ip( 192, 168, 10, 1 );
IPAddress subnet( 255, 255, 255, 0 );


void ForwardOn() {
  digitalWrite(GPIO_FON, HIGH);
  delay(500);
  digitalWrite(GPIO_FON, LOW);
  String html = HTML_HEADER "<h1>正転運転を実行しました。</h1><br><br><br>"
                HTML_GOTOTOP
                HTML_FOOTER;
  server.send(200, "text/html", html);
  Serial.println("Go! Forward.正転を実行しました。");
}


void ReverseOn() {
  digitalWrite(GPIO_RON, HIGH);
  delay(500);
  digitalWrite(GPIO_RON, LOW);
  String html = HTML_HEADER "<h1>逆転運転を実行しました。</h1><br><br><br>"
                HTML_GOTOTOP
                HTML_FOOTER;
  server.send(200, "text/html", html);
  Serial.println("Go! Reverse.逆転を実行しました。");
}


void AllStop() {
  digitalWrite(GPIO_STOP, HIGH);
  delay(500);
  digitalWrite(GPIO_STOP, LOW);
  String html = HTML_HEADER "<h1>停止を実行しました。</h1><br><br><br>"
                HTML_GOTOTOP
                HTML_FOOTER;
  server.send(200, "text/html", html);
  Serial.println("Go! Reverse.停止を実行しました。");
}


void setup() {
  pinMode(GPIO_STOP, OUTPUT);
  pinMode(GPIO_FON, OUTPUT);
  pinMode(GPIO_RON, OUTPUT);
  Serial.begin(115200);
  delay(500);
  Serial.println("");
  Serial.println("Start http");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(WIFI_SSID, WIFI_PWD);
  server.on("/fon", ForwardOn);
  server.on("/ron", ReverseOn);
  server.on("/stp", AllStop);
  server.on("/", []() {
    String html = HTML_HEADER "<h2>インバータコントローラ</h2><br><h2>動作を選択してください。</h2><hr><br><BR>"
                  "<button onclick=\"location.href='http://192.168.10.1/fon'\">(正転)運転開始)</button><A></A><BR><BR>"\
                  "<button onclick=\"location.href='http://192.168.10.1/ron'\">(逆転)運転開始</button><A></A><BR><BR>"
                  "<button onclick=\"location.href='http://192.168.10.1/stp'\">(停止)停　止</button><br><BR><BR><hr><BR>"
                  "<a>注：この接続は電動機制御のためのクローズ接続です。不必要な場合は、必ずこのWiFiの設定を削除してください。</a><br> "
                  "<a>このWiFi圏内にいるとインターネットに接続不能となります。ご注意くだいませ( ^^) _U~~</a><br><br><br><br><br> "
                  "<a>技適認証番号[R]217-204070</a><br> "
                  "<a>SoC:ESP-WROOM-02 2020.12<br><br>オープンソースコードは、WiFi切断して、"
                  "</a><a href='https://github.com/yamarikana/'>こちらから</a><a>取得可能です。（いつまでか保証なし！）</a><br><br> "
                  "<a>Firmware 1.0.7</a><br> "
/*
                  "<button onclick=\"location.href='http://192.168.10.1/spdl'\">設定済み(低速運転)</button><br> "
                  "<button onclick=\"location.href='http://192.168.10.1/spdm'\">設定済み(中速運転)</button><br> "
                  "<button onclick=\"location.href='http://192.168.10.1/spdh'\">設定済み(高速運転)</button><br><hr><br>"
*/
                  HTML_FOOTER;
    server.send(200, "text/html", html);
  });
  server.begin();
}

void loop() {
  server.handleClient();
}
