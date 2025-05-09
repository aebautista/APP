from flask import Flask, request, jsonify
import requests  # Usado para comunicarse con el ESP32 (si tiene IP pública o local)

app = Flask(__name__)

# Dirección IP del ESP32 
ESP32_IP = 'http://192.168.1.50'  # Ejemplo

@app.route('/abrir_puerta', methods=['POST'])
def abrir_puerta():
    data = request.get_json()
    comando = data.get('comando', '').lower()

    if comando == 'abrir puerta':
        # Enviar señal al ESP32 (puede ser una petición GET o POST)
        try:
            response = requests.get(f'{ESP32_IP}/abrir')  # ESP32 debe tener un endpoint '/abrir'
            return jsonify({'mensaje': 'Puerta abierta', 'respuesta_esp32': response.text}), 200
        except Exception as e:
            return jsonify({'error': str(e)}), 500
    else:
        return jsonify({'mensaje': 'Comando no reconocido'}), 400

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)