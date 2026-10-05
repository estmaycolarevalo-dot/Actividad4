import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision
import serial
import time

# --- REEMPLAZA CON EL PUERTO COM QUE TE MOSTRÓ ARDUINO IDE ---
PUERTO_SERIAL = 'COM5' 

try:
    esp32 = serial.Serial(PUERTO_SERIAL, 115200, timeout=1)
    time.sleep(2)
    print(f"Conectado exitosamente a la ESP32 en {PUERTO_SERIAL}")
except Exception as e:
    print(f"Error conectando a la ESP32: {e}")
    esp32 = None

# --- CONFIGURACIÓN MEDIAPIPE ---
MODEL_PATH = 'gesture_recognizer.task'
base_options = python.BaseOptions(model_asset_path=MODEL_PATH)
options = vision.GestureRecognizerOptions(
    base_options=base_options,
    num_hands=2,
    running_mode=vision.RunningMode.IMAGE
)
recognizer = vision.GestureRecognizer.create_from_options(options)

ultimo_comando = ""

def enviar_comando(cmd):
    global ultimo_comando
    if cmd != ultimo_comando:
        print(f"Enviando comando: {cmd}")
        if esp32 and esp32.is_open:
            esp32.write(f"{cmd}\n".encode())
        ultimo_comando = cmd

# --- BUCLE DE CÁMARA ---
cap = cv2.VideoCapture(0)

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb_frame)

    result = recognizer.recognize(mp_image)

    gestos_detectados = []
    if result.gestures:
        for gesture in result.gestures:
            gestos_detectados.append(gesture[0].category_name)

    # Lógica según la rúbrica del ejercicio
    if len(gestos_detectados) == 2 and all(g == "Open_Palm" for g in gestos_detectados):
        enviar_comando("MODE_100")
    elif len(gestos_detectados) >= 1:
        gesto = gestos_detectados[0]
        if gesto == "Closed_Fist":
            enviar_comando("MODE_30")
        elif gesto == "Victory":
            enviar_comando("MODE_70")
        elif gesto == "Thumb_Down":
            enviar_comando("SEQ_1")
        elif gesto == "Thumb_Up":
            enviar_comando("SEQ_2")

    cv2.imshow("Control por Gestos - ESP32", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
if esp32:
    esp32.close()