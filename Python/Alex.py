logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s - %(message)s - %(processName)s',
)

""" ---- Modulo ingresado ---- """
    
  def preguntar(self):
        if self.clk:
            print("Es secuencial")
        else:
            print("Es combinacional")
            self.circuito = Combinacional(entradas=self.e_s)
            self.variables, num = self.circuito.mapear_variables()
            self.variables_utt = self.circuito.definir_parametros_uut(self.variables)
            print(f"El largo es {num}")
            if 0 < num < 6:
                self.signals = self.circuito.definir_entradas()
            else:
                print(f"Son muchas variables. {2 ** num} posibles casos")
                self.signals = self.circuito.leer_variables_usuario(self.doc_cri)

  def leer_variables_usuario(self, doc):
        try:
            with open(doc, "r") as user_entry:
                data = user_entry.readlines()    # Lee el documento por lineas
                logging.debug(f"El usuario ingreso: \n {data}")
        except:
            print(f"Algo salio mal leer el archivo")
            data = ""
        finally:
            salida = ""
            for valor in self.e_s[0]:
                pattern = r"[0-9](:)[0-9]"
                if re.findall(pattern, valor):
                    aux = valor[valor.find(']') + 2:].split(',')
                else:
                    aux = valor.split(',')
                    print(f"Los valores son: {aux} y el largo es{len(aux)}")
                    for numero in range(0, 10):
                        for lugar in range(0, len(aux)):
                            salida = salida + f"{aux[lugar]}= {random.randint(0, 1)}; "
                        salida = salida + " #1\n"
            logging.debug(f"Lo random es: \n{salida}")
            return "".join(data) + salida
