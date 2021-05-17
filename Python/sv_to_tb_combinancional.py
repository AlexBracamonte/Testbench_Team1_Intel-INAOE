import re
import os
import sys


class GeneradorTest:
    def __init__(self, name: str, out_name:str):
        self.Nombre_Documento = name
        self.out_name = out_name
        self.lineas = None
        self.documento = None

        self.nombre_modulo = ""
        self.e_s = [[], [], []]  # ['input', 'output', 'inout']

    def leer_documento(self):
        try:
            with open(self.Nombre_Documento, "r") as design:
                self.lineas = design.readlines()    # Lee el documento por lineas
                design.seek(0)
                self.documento = design.read()
                print(self.documento)
        except:
            print(f"Algo salio mal leer el archivo")
            sys.exit(1)

    def Obtener_Nombre_Modulo(self):
        pattern = r"module[\s]+(\S*)[\s]*\([^\)]*\)[\s\S]*"
        for linea in self.lineas:
            if re.findall(pattern, linea):
                self.nombre_modulo = re.findall(pattern, linea)[0].strip('(),;')
                print(self.nombre_modulo)

    def obtener_io(self):
        GPIO = ['input', 'output', 'inout']
        for valor in GPIO:
            pattern = f"({valor})[\s]+([^;,\)]+)[\s]*[;,]"
            for linea in self.lineas:
                if re.findall(pattern, linea):
                    aux = linea.replace(valor, '')
                    aux = aux.strip(' \n;()')     # Eliminar el \n y el signo ;
                    self.e_s[GPIO.index(valor)].append(aux)

    def escribe_TB(self):
        variables = self.mapear_variables()
        with open(f"{self.out_name}", "w+") as testbench:
            testbench.write("`timescale 1ns/1ns\n\n")
            testbench.write(f"module {self.nombre_modulo}_TB;\n")
            for valor in self.e_s[0]:
                testbench.write(f"\treg {valor};\n")
            for valor in self.e_s[1]:
                testbench.write(f"\twire {valor};\n")
            testbench.write('\n')
            testbench.write(f"\t{self.nombre_modulo} UUT ({variables});\n\n")

            print(f"Las variables son: {variables} y si {variables.find('clk')}")

            if variables.find('clk') != -1:   #Preguntamos por CLK
                testbench.write(f"\talways #1 clk = ~clk;\n\n")

            testbench.write(f'initial begin\n'
                            f'\t$dumpfile("{self.nombre_modulo}.vcd");\n'
                            f'\t$dumpvars(1, {self.nombre_modulo}_TB);\n')

            if variables.find('clk') != -1:   #Preguntamos por CLK
                testbench.write(f"\tclk <= 0;\n\n")

            testbench.write(f"\t{self.definir_entradas()}\n\n")

            testbench.write(f'\t$finish;\n'
                            f'end\n'
                            f'endmodule')

    def mapear_variables(self) -> str:
        aux = ""
        aux2 = ""
        for valor in self.e_s[0]:
            pattern = r"[0-9](:)[0-9]"
            if re.findall(pattern, valor):
                print(valor[valor.find(']') + 2:])
                aux2 = aux2 + (valor[valor.find(']') + 2:] + ', ')
            else:
                aux2 = "".join(valor) + ', '
        aux = aux + aux2
        print(aux2)

        aux2 = ""
        for valor in self.e_s[1]:
            pattern = r"[0-9](:)[0-9]"
            if re.findall(pattern, valor):
                print(valor[valor.find(']') + 2:])
                aux2 = aux2 + (valor[valor.find(']') + 2:] + ', ')
            else:
                aux2 = "".join(valor) + ', '
        print(aux2)

        aux = aux + aux2[:-2]
        print(f"las variables mapeadas son: {aux}")
        return aux

    def definir_entradas(self):
        salida = ""
        for valor in self.e_s[0]:
            pattern = r"[0-9](:)[0-9]"
            if re.findall(pattern, valor):
                aux = valor[valor.find(']') + 2:].split(',')
                print(f"asdasdasdasdsagfg {aux}")
            else:
                aux = valor.split(',')
                if f" clk" in aux:
                    aux.remove(' clk')
                if f"clk" in aux:
                    aux.remove('clk')

                largo = 2 ** len(aux)
                for numero in range(0, largo):
                    # print(format(numero, f"0{str(len(aux))}b"))
                    num_in_bin = format(numero, f"0{str(len(aux))}b")
                    for lugar in range(0, len(aux)):
                        salida = salida + f"{aux[lugar]}= {num_in_bin[lugar]}; "
                    salida = salida + " #1\n"
        # print(salida)
        return salida





if __name__ == '__main__':
    test1 = GeneradorTest(name="design.sv", out_name="test.sv")
    test1.leer_documento()
    test1.Obtener_Nombre_Modulo()
    test1.obtener_io()
    test1.definir_entradas()
    test1.escribe_TB()
