# Distributed Graph Attention Network with TCP in C++

Bienvenidos al repositorio **CPlusPlusGraphNet**. A continuación, encontrarás las instrucciones necesarias para colaborar en este proyecto de forma organizada. 

---

## 🛠️ Pasos iniciales para colaborar

### 1. Clonar el repositorio
1. Verifica que tengas **Git** instalado en tu sistema. Si no lo tienes, [descárgalo aquí](https://git-scm.com/).
2. Abre tu terminal o consola de comandos.
3. Ejecuta este comando para descargar una copia del repositorio:
   ```bash
   git clone https://github.com/felix-gy/CPlusPlusGraphNet.git
   ```
4. Navega a la carpeta del proyecto:
   ```bash
   cd CPlusPlusGraphNet
   ```

---

### 2. Cambiar a la rama `develop`
Todos trabajaremos en la rama `develop` para mantener el flujo organizado:
1. Cambia a la rama `develop`:
   ```bash
   git checkout develop
   ```
2. Antes de realizar cualquier cambio, asegúrate de que tu copia esté actualizada:
   ```bash
   git pull origin develop
   ```

---

### 3. Hacer y guardar tus cambios
1. Modifica los archivos necesarios.
2. Guarda los cambios que realizaste:
   ```bash
   git add .
   ```
   O si prefieres agregar solo un archivo específico:
   ```bash
   git add archivo.cpp
   ```
3. Crea un registro de tus cambios con un mensaje descriptivo:
   ```bash
   git commit -m "Descripción breve del cambio"
   ```
   Ejemplo:
   ```bash
   git commit -m "Añadida función para manejo de conexiones TCP"
   ```

---

### 4. Subir tus cambios al repositorio
1. Envía tus cambios a la rama `develop`:
   ```bash
   git push origin develop
   ```
2. Comunica al equipo los cambios realizados.

---

### 5. Mantener tu copia actualizada
Antes de comenzar a trabajar, siempre sincroniza tu copia local con los últimos cambios en `develop`:
1. Asegúrate de estar en la rama `develop`:
   ```bash
   git checkout develop
   ```
2. Descarga los cambios más recientes:
   ```bash
   git pull origin develop
   ```

---

## 🏗️ Cómo compilar y ejecutar el proyecto

1. Asegúrate de estar en la carpeta raíz del proyecto.
2. Usa el comando `make` para compilar:
   ```bash
   make
   ```
3. Ejecuta el programa:
   ```bash
   ./nombre-del-ejecutable
   ```
   (El nombre del ejecutable dependerá del archivo `Makefile`.)
4. Limpia los archivos generados si es necesario:
   ```bash
   make clean
   ```

---

## 📖 Recursos útiles

- [Documentación oficial de Git](https://git-scm.com/doc)
- [Guía básica de Markdown](https://www.markdownguide.org/)
- [Tutorial de GitHub para principiantes](https://docs.github.com/es/get-started/quickstart)

---

🚀
