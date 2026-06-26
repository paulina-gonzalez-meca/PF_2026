#include "Verificador.h"

bool esNumerico(String str) {
  // REQUERIMIENTO: Si es exactamente "-", se considera un dato vacío VÁLIDO.
  if (str == "-") {
    return true;
  }

  int longitud = str.length();
  if (longitud == 0) return false;  // Un string totalmente vacío no es válido

  int inicio = 0;

  // REQUERIMIENTO: Permitir números negativos.
  // Si el primer carácter es un menos, empezamos a validar los dígitos desde la posición 1.
  if (str.charAt(0) == '-') {
    inicio = 1;
    if (longitud == 1) return false;  // Un '-' solo (sin números) no es válido (ya se filtró el "-" de vacío arriba)
  }

  bool tienePuntoDecimal = false;

  // Este bucle toma microsegundos en el ESP32, es 100% seguro para tu FSM
  for (int i = inicio; i < longitud; i++) {
    char c = str.charAt(i);

    if (c == '.') {
      // Si ya encontramos un punto decimal antes, un segundo punto es un error (ej: 12.3.4)
      if (tienePuntoDecimal) return false;
      tienePuntoDecimal = true;
    } else if (!isDigit(c)) {
      // Si no es un dígito (0-9) ni tampoco un punto decimal, el dato está corrupto
      return false;
    }
  }

  // Si pasó todos los filtros, el número es completamente válido
  return true;
}