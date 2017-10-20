void divisionEntera(unsigned int dividendo1, unsigned int divisor1, unsigned int* resultado[]) {
	unsigned int denominador = divisor1;
	unsigned int temp = 1;
	unsigned int temp2 = 0x80000000;
	unsigned int resultadoParteEntera = 0;
	unsigned int resultadoFraccion = 0;
	unsigned int dividendo = dividendo1;
	unsigned int divisor = divisor1;

    //  Se calcula cuantas posiciones enteras tendr� el resultado
	while (denominador <= dividendo) {
		denominador <<= 1;
		temp <<= 1;
	}

	denominador >>= 1;
	temp >>= 1;

	while (temp != 0) {
		if (dividendo >= denominador) {
			dividendo -= denominador;
			resultadoParteEntera |= temp;
		}
		temp >>= 1;
		denominador >>= 1;
	}

    // se procede a calcular la parte decimal del resultado
	if (dividendo > 0) {
		dividendo <<= 1;
		int count = 23;
		while (dividendo > 0 && count > 0) {
			if (dividendo >= divisor) {
				dividendo -= divisor;
				resultadoFraccion |= temp2;
			}
			temp2 >>= 1;
			dividendo <<= 1;
			count--;
		}
	}
    
    // se guarda el resultado entero y fraccional en un arreglo
	resultado[0] = resultadoParteEntera;
	resultado[1] = resultadoFraccion;
}

void probarDivision(unsigned int operando1, unsigned int operando2) {
	unsigned int resultado[2] = { 0, 0 };
	divisionEntera(operando1, operando2, &resultado);
}

int main()
{
	probarDivision(43, 4);
}
