
// Seu sketch deve #incluir essa biblioteca e a biblioteca Wire.
// (Wire é uma biblioteca padrão incluída no Arduino):
#include <SFE_BMP180.h>
#include <Wire.h>

// Você precisará criar um objeto SFE_BMP180, aqui chamado de “pressure”:

SFE_BMP180 pressure;

#define ALTITUDE 1655.0 // Altitude da sede da SparkFun em Boulder, CO. em metros

void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");

  // Inicializar o sensor (é importante obter os valores de calibração armazenados no dispositivo).
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Oops, algo deu errado, geralmente é um problema de conexão,
    // veja os comentários na parte superior desse esboço para as conexões corretas.
    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause forever.
  }
}

void loop()
{
  char status;
  double T,P,p0,a;

  // Faça um loop aqui obtendo leituras de pressão a cada 10 segundos.

  // Se você quiser a pressão compensada pelo nível do mar, como usada em relatórios meteorológicos,
  // você precisará saber a altitude em que suas medições são feitas.
  // Nesse esboço, estamos usando uma constante chamada ALTITUDE:
  
  Serial.println();
  Serial.print("provided altitude: ");
  Serial.print(ALTITUDE,0);
  Serial.print(" meters, ");
  Serial.print(ALTITUDE*3.28084,0);
  Serial.println(" feet");
  
  // Se você quiser medir a altitude, e não a pressão, precisará
  // fornecer uma pressão de linha de base conhecida. Isso é mostrado no final do esboço.

  // Você deve primeiro obter uma medição de temperatura para realizar uma leitura de pressão.
  
  // Iniciar uma medição de temperatura:
  // Se a solicitação for bem-sucedida, o número de ms a esperar é retornado.
  // Se a solicitação não for bem-sucedida, será retornado 0.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Aguarde a conclusão da medição:
    delay(status);

    // Recuperar a medição de temperatura concluída:
    // Observe que a medição é armazenada na variável T.
    // A função retorna 1 em caso de sucesso e 0 em caso de falha.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Imprima a medição:
      Serial.print("temperature: ");
      Serial.print(T,2);
      Serial.print(" deg C, ");
      Serial.print((9.0/5.0)*T+32.0,2);
      Serial.println(" deg F");
      
      // Iniciar uma medição de pressão:
      // O parâmetro é a configuração de sobreamostragem, de 0 a 3 (resolução mais alta, espera mais longa).
      // Se a solicitação for bem-sucedida, o número de ms a esperar é retornado.
      // Se a solicitação não for bem-sucedida, será retornado 0.
      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Aguarde a conclusão da medição:
        delay(status);

        // Recuperar a medição de pressão concluída:
        // Observe que a medição é armazenada na variável P.
        // Observe também que a função requer a medição de temperatura anterior (T).
        // (Se a temperatura for estável, é possível fazer uma medição de temperatura para várias medições de pressão).
        // A função retorna 1 em caso de sucesso e 0 em caso de falha.
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          // Imprima a medição:
          Serial.print("absolute pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P*0.0295333727,2);
          Serial.println(" inHg");

          // O sensor de pressão retorna a pressão absoluta, que varia com a altitude.
          // Para remover os efeitos da altitude, use a função sealevel e sua altitude atual.
          // Esse número é comumente usado em relatórios meteorológicos.
          // Parâmetros: P = pressão absoluta em mb, ALTITUDE = altitude atual em m.
          // Resultado: p0 = pressão compensada ao nível do mar em mb

          p0 = pressure.sealevel(P,ALTITUDE); // we're at 1655 meters (Boulder, CO)
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0,2);
          Serial.print(" mb, ");
          Serial.print(p0*0.0295333727,2);
          Serial.println(" inHg");

          // Por outro lado, se você quiser determinar sua altitude a partir da leitura da pressão,
          // use a função de altitude junto com uma pressão de linha de base (nível do mar ou outra).
          // Parâmetros: P = pressão absoluta em mb, p0 = pressão de linha de base em mb.
          // Resultado: a = altitude em m.

          a = pressure.altitude(P,p0);
          Serial.print("computed altitude: ");
          Serial.print(a,0);
          Serial.print(" meters, ");
          Serial.print(a*3.28084,0);
          Serial.println(" feet");
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");

  delay(5000);  // Pause for 5 seconds.
}