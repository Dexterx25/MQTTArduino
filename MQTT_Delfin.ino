int led = 4; // LED conectado a D0
int pushButton = 2; // Botón conectado a D2
// Esta función sólo corre una vez cuando enciende/reinicia 
void setup()
{
pinMode(led, OUTPUT); // Inicializa D0 como salida
pinMode(pushButton, INPUT_PULLUP);
// Inicializa D2 como entrada conectada a una resistencia Pull-up interna
}
// Esta función corre para siempre
void loop()
{
int pushButtonState;
pushButtonState = digitalRead(pushButton);
  if(pushButtonState == LOW)
  { // Si aprietas el boton...
    digitalWrite(led, HIGH);  // Prende el LED
  }
  else // Si lo sueltas
  {
  digitalWrite(led, LOW);   // Apaga el LED
} }
