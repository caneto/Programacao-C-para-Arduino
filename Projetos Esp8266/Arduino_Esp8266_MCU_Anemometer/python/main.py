from machine import ADC, Pin
from time import sleep

# Constantes
min_voltage = 0.033  # Tensão correspondente a 0 m/s
max_voltage = 3.3    # Tensão correspondente a 32,4 m/s (velocidade máxima quando se utiliza o divisor de tensão)
max_wind_speed = 32.4 # Velocidade máxima do vento em m/s

# Factores de conversão
mps_to_kmh = 3.6     # 1 m/s = 3.6 km/h
mps_to_mph = 2.23694 # 1 m/s = 2.23694 mph

# Configurar o ADC
adc = ADC(0)

while True:
    # Ler o valor analógico do anemómetro (valor ADC entre 0-1023)
    adc_value = adc.read()
    
    # Converter o valor ADC em tensão
    voltage = (adc_value / 1023.0) * 3.3
    
    # Assegurar que a tensão está dentro do intervalo de funcionamento do anemómetro
    if voltage < min_voltage:
        voltage = min_voltage
    elif voltage > max_voltage:
        voltage = max_voltage
    
    # Mapear a tensão para a velocidade do vento
    wind_speed_mps = ((voltage - min_voltage) / (max_voltage - min_voltage)) * max_wind_speed

    # Converter a velocidade do vento em km/h e mph
    wind_speed_kmh = wind_speed_mps * mps_to_kmh
    wind_speed_mph = wind_speed_mps * mps_to_mph

    # Imprimir velocidade do vento
    print("Wind Speed:")
    print("{:.2f} m/s".format(wind_speed_mps))
    print("{:.2f} km/h".format(wind_speed_kmh))
    print("{:.2f} mph".format(wind_speed_mph))
    print() 
    
    sleep(1)