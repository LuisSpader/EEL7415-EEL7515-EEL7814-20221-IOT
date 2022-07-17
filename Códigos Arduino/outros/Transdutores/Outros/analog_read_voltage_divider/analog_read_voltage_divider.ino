int valor = 0;
float valor_float = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  // Faz a leitura das tensões de interesse:
  valor = analogRead(A0);
  valor_float = float(map(valor, 0, 1023, 0, 5000))/1000;
  Serial.println(valor_float);
}
