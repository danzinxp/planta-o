
// ==========================================
//             AGROMONITOR
// Arduino UNO + Sensor + LEDs + Bomba
// Comunicacao USB com o site
// ==========================================

const int sensorUmidade = A0;

const int releBomba = 9;

const int ledVermelho = 10;
const int ledVerde = 11;
const int ledAzul = 12;

// Limites iniciais de umidade
// Ajustar conforme as leituras reais
const int limiteSeco = 700;
const int limiteEncharcado = 400;

// Configuracao do modulo rele
// Padrao: rele acionado com LOW
const int RELE_LIGADO = LOW;
const int RELE_DESLIGADO = HIGH;

// Temporizadores
unsigned long tempoLeitura = 0;
unsigned long tempoPisca = 0;

const unsigned long intervaloLeitura = 1000;
const unsigned long intervaloPisca = 150;

bool estadoLedAzul = false;

String estadoSolo = "";

void setup() {

  Serial.begin(9600);

  // Configura os pinos
  pinMode(sensorUmidade, INPUT);

  pinMode(releBomba, OUTPUT);

  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);

  // Garante que a bomba inicie desligada
  digitalWrite(releBomba, RELE_DESLIGADO);

  // Inicia os LEDs apagados
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAzul, LOW);

  Serial.println("AGROMONITOR INICIADO");
}

void loop() {

  unsigned long agora = millis();

  // ==========================================
  // LEITURA DO SENSOR A CADA 1 SEGUNDO
  // ==========================================

  if (agora - tempoLeitura >= intervaloLeitura) {

    tempoLeitura = agora;

    int leitura = analogRead(sensorUmidade);

    // ======================================
    // SOLO SECO
    // ======================================

    if (leitura >= limiteSeco) {

      estadoSolo = "SECO";

      digitalWrite(ledVermelho, HIGH);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledAzul, LOW);

      // LIGA A BOMBA
      digitalWrite(releBomba, RELE_LIGADO);

    }

    // ======================================
    // SOLO ENCHARCADO
    // ======================================

    else if (leitura <= limiteEncharcado) {

      estadoSolo = "ENCHARCADO";

      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, LOW);

      // DESLIGA A BOMBA
      digitalWrite(releBomba, RELE_DESLIGADO);

    }

    // ======================================
    // UMIDADE IDEAL
    // ======================================

    else {

      estadoSolo = "IDEAL";

      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledAzul, LOW);

      // DESLIGA A BOMBA
      digitalWrite(releBomba, RELE_DESLIGADO);

    }

    // ======================================
    // ENVIA DADOS PARA O SITE
    // Formato: AGRO,LEITURA,ESTADO
    // ======================================

    Serial.print("AGRO,");
    Serial.print(leitura);
    Serial.print(",");
    Serial.println(estadoSolo);

  }

  // ==========================================
  // PISCA LED AZUL NO SOLO ENCHARCADO
  // ==========================================

  if (estadoSolo == "ENCHARCADO") {

    if (agora - tempoPisca >= intervaloPisca) {

      tempoPisca = agora;

      estadoLedAzul = !estadoLedAzul;

      digitalWrite(ledAzul, estadoLedAzul);

    }

  } else {

    estadoLedAzul = false;

    digitalWrite(ledAzul, LOW);

  }

}
