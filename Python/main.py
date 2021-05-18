import os
from basico1_TB import GeneradorTest

if __name__ == '__main__':
    dir = "sv"
    dir_sal = "salidas"
    contenido = os.listdir(dir)
    objetos_generados = []
    for archivo in contenido:
        salida = f"testbench_{archivo}"
        print(f"El nombre es {archivo} y el tipo es {type(archivo)} y el de salida es {salida}")
        archivo_objeto = GeneradorTest(name=f"{dir}\\{archivo}", out_name=f"{dir_sal}\\{salida}")
        objetos_generados.append(archivo_objeto)

    for objeto in objetos_generados:
        objeto.leer_documento()
        objeto.Obtener_Nombre_Modulo()
        objeto.obtener_io()
        objeto.definir_entradas()
        objeto.escribe_TB()
